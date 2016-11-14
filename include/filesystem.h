#ifndef SIMPLEBASIC_FILESYSTEM_H
#define SIMPLEBASIC_FILESYSTEM_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

extern int maxInode = 0;

#pragma pack(push, 2)
typedef struct file{
    struct file *parent;
    int inode;
    char *name;
    char type;
    size_t actualSize;
    size_t usedSize;
    void *content;
}file;
#pragma pack(pop)

typedef struct record{
    file *previous;
    /*
     * previous == NULL - сама директория
     */
    file *next;
    /*
     * next == NULL - последний файл в списке.
     */

    file *current;
};

#define DEFAULT_INCREASE 8


#endif
