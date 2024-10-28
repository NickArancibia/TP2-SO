#ifndef __QUEUEADT_H
#define __QUEUEADT_H

#include <process.h>

typedef PID QueueType;

typedef struct QueueCDT *QueueADT;

QueueADT newQueue(void);

void queue(QueueADT queue, QueueType elem);

QueueType dequeue(QueueADT queue);

int isEmpty(QueueADT queue);

void freeQueue(QueueADT queue);

#endif