#include <signal.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "filesystem.h"
#include "interpretator.h"
#include "syscalls.h"

volatile sig_atomic_t scheduler_flag = false;
volatile sig_atomic_t term_flag = false;
volatile sig_atomic_t stop_flag = false;

interpretator_state* current_state;
interpretator_state proc[256];
size_t proc_count = 0;
size_t proc_current = 0;
size_t proc_foreground = 0;

file *workingDirectory;

void interrupt_handler(interpretator_state *state) {
	if(scheduler_flag) {
		do {
			proc_current = (proc_current + 1) % 256;
		} while(proc[proc_current].status == PROC_KILLED || proc[proc_current].status == PROC_STOPPED);
		scheduler_flag = false;
		current_state = &proc[proc_current];
	}
	if(term_flag) {
		term_flag = false;
		syscalls_kill_verbose(proc_foreground);
		printf("sh > ");
		fflush(stdout);
		proc_foreground = 0;
	}
	if(stop_flag) {
		stop_flag = false;
		proc[proc_foreground].status = PROC_STOPPED;
		printf("[%li] Stopped\n", proc_foreground);
		printf("sh > ");
		fflush(stdout);
		proc_foreground = 0;
	}
}

void timer_handler(int sig) {
	scheduler_flag = true;
}

void term_handler(int sig) {
	term_flag = true;
}

void stop_handler(int sig) {
	stop_flag = true;
}


int main() {
	struct sigaction sa;
	struct itimerval timer;
	signal(SIGINT, term_handler);
	signal(SIGTSTP, stop_handler);

	/* Install timer_handler as the signal handler for SIGVTALRM. */
	memset (&sa, 0, sizeof (sa));
	sa.sa_handler = &timer_handler;
	sigaction (SIGPROF, &sa, NULL);
	/* Configure the timer to expire after 25 msec... */
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 250000;
	/* ... and every 250 msec after that. */
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 250000;
	workingDirectory = initFileSystem();
	char inputBody[] = "jobs\nend";
	file *input = newFile(workingDirectory, "input", '-', lastRecord(workingDirectory));
	addContent(input, inputBody, sizeof(inputBody));
	
	for(size_t i = 0; i < 256; i++) {
		proc[i].status = PROC_KILLED;
	}
	
	syscalls_exec(NULL);
	current_state = &proc[0];




	printf("sh > ");
	fflush(stdout);
	setitimer (ITIMER_PROF, &timer, NULL);
	while(proc_count)
		launchInterpretator(current_state);
	
	return 0;
}