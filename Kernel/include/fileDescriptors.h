#ifndef _PIPES_H
#define _PIPES_H
#define MAX_FDS 1024

#include "semaphore.h"

#define BUFFER_SIZE 1024
#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef enum
{
    R = 1,
    W = 2,
    RW = 3
} Mode;

typedef struct
{
    char buffer[BUFFER_SIZE];
    int readPos;
    int writePos;
    sem_t readSem;
    sem_t writeSem;
} Stream;


typedef struct
{
    Stream * resource;
    int isOpen;
    Mode mode;
} FD;

void initFileDescriptors();
int writeToFD(int fd, char *buf, uint64_t count,uint64_t hexColor);
int readFromFD(int fd, char *buf, uint64_t count);
int createPipe(int fds[2]);

#endif