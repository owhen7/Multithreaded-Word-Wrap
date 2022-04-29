#ifndef WW_H
#define WW_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#define DEBUG 1
#define BUFSIZE 10

int isDirectory(const char *path);
int wordWrapTextFile(char* argument3, int wrapWidth);
//void enqueue(struct dirQueue* directory_queue, char item);
//char dequeue(struct dirQueue* directory_queue);
//void* directoryWorker(void* initialPath, struct dirQueue* directory_queue, struct fileQueue* file_queue);

struct dirQueue {
    int start;
    int stop;
    int front;
    int rear;
    int size;
    char* array;
}dirQueue;

struct fileQueue {
    int start;
    int stop;
    int front;
    int rear;
    int size;
    unsigned capacity;
    char* array;
}fileQueue;

struct argStruct {
    void* initialPath;
    struct dirQueue* directory_queue;
    struct fileQueue* file_queue;
    int readThreads;
}argStruct;

#define MEMSIZE1 30000
#define MEMSIZE2 30000
#endif
