#ifndef _PIPES_H
#define _PIPES_H
#define MAX_PIPES 10

#include "semaphore.h"

typedef struct {
    
}  endPipe;


typedef struct {
    int read;
    int write;
} pipe_t;


#endif