#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "./process.h"
#include <stdint.h>
typedef struct Node
{
    Process * pcb;
    struct Node * next;
    uint32_t executionsLeft;
}Node;

typedef struct List
{
    Node * head ;
    Node * tail ;
}List;


void initInitScheduler();
void initScheduler();

void schedule(Process * pcb);
Process * unschedule();
uint64_t * switchContent(uint64_t * rsp);

Process * getCurrentProcess();

#endif