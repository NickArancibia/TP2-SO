#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <scheduler.h>
#include <process.h>
#include "interrupts.h"

#define MAX_SEMAPHORES 200
typedef int sem_t;
int initializeSems();

int semOpen( char *sem_id, int initialValue );

int semWait( int sem_id );

int semPost( int sem_id );

int semClose( int sem_id );

#endif