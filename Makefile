CC=gcc
IDIR=./include
CFLAGS= -pedantic -Wall -I$(IDIR)
BDIR=./build
LIBS= -lm
LDK = -o
CDK= -c
TARGET=main
SRC=$(wildcard src/*.c)
OBJ=$(SRC:src/%.c=build/%.o)
FILES = main.c

mainmake: $(OBJ)
	$(CC) $(LDK) $(TARGET) $(OBJ)

build/%.o: src/%.c 
	$(CC) $(CDK) $(LDK) $@ $< $(CFLAGS)


#clean:
#	rm -rf $(TARGET)

