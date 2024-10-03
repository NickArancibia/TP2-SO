// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/memoryManager.h"
#include "./include/scheduler.h"

Quantum qtyQuantums;
List list;
Process *currentProcess = NULL;

void initInitScheduler()
{
    list.head = NULL;
    list.tail = NULL;
}

void initScheduler()
{
    currentProcess = list.head->pcb;
}

void schedule(Process *pcb)
{
    Node *node = mallocMM(sizeof(Node));
    node->pcb = pcb;

    if (list.head == NULL)
    {
        list.head = node;
        list.tail = node;
        node->next = list.head;
    }
    else
    {
        node->next = list.head;
        list.tail->next = node;
        list.tail = node;
    }
}

Process *unschedule()
{
    if (list.head == NULL)
    {
        return NULL;
    }

    Process *pcb = list.head->pcb;

    if (list.head == list.tail)
    {
        freeMM(list.head);
        list.head = NULL;
        list.tail = NULL;
    }
    else
    {
        Node *temp = list.head;
        list.head = list.head->next; // Move head to the next node
        list.tail->next = list.head; // Update tail to point to the new head (circular link)
        freeMM(temp);
    }
    return pcb;
}

uint64_t *switchContent(uint64_t *rsp)
{
    /*
    if(ticksGlobalElapsed <  0  pcb->ticksElapsed ){
        ticksGlobalElapsed++;
        return rsp;
    }
    */
    if (currentProcess == NULL)
    {
        return rsp;
    }
    if (currentProcess->state == RUNNING)
    {
        currentProcess->stackEnd = rsp;
        schedule(currentProcess);
        currentProcess->state = READY;
    }
    currentProcess = unschedule();

    currentProcess->state = RUNNING;
    return currentProcess->stackEnd;
}

Process *getCurrentProcess()
{
    return currentProcess;
}
