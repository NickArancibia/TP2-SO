#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#include <scheduler.h>
#include <process.h>
#include "interrupts.h"

#define MAX_SEMAPHORES 15

int initializeSems();

int semOpen( char *sem_id, int initialValue );

int semWait( char *sem_id );

int semPost( char *sem_id );

int semClose( char *sem_id );

#endif