// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/memoryManager.h"
#include "./include/scheduler.h"
#include "./include/syscallHandle.h"

Quantum qtyQuantums;
List list;
Process *currentProcess = NULL;
char isYield = YIELD_NOT_DONE;

void initScheduler()
{
    currentProcess = list.head->pcb;
}

void schedule(Process *pcb)
{
    Node *node = mallocMM(sizeof(Node));
    node->pcb = pcb;
    node->executionsLeft = pcb->priority - 1;

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
    if (currentProcess == NULL)
    {
        return rsp;
    }

    if(list.head->executionsLeft > 0 && getYield() != YIELD_DONE)
    {
        list.head->executionsLeft--;
        return rsp;
    }

    if (currentProcess->state == RUNNING)
    {
        currentProcess->stackEnd = rsp;
        schedule(currentProcess);
        currentProcess->state = READY;
    }
    if (currentProcess->state == BLOCKED)
    {
        currentProcess->stackEnd = rsp;
    }
    do
    {
        currentProcess = unschedule();

        if (currentProcess == NULL)
        {
            return rsp;
        }
    } while(currentProcess->state == BLOCKED || currentProcess->state == DEAD);

    currentProcess->state = RUNNING;
    return currentProcess->stackEnd;
}

Process *getCurrentProcess()
{
    return currentProcess;
}

int blockProcess(PID pid)
{
    Process *pcb;
    if ((pcb = getProcess(pid)) == NULL)
        return 1;
    pcb->state = BLOCKED;
    if (pcb->pid == currentProcess->pid)
        return yield();
    return 0;
}

int unblockProcess(PID pid)
{
    Process *pcb = getProcess(pid);
    pcb->state = READY;
    schedule(pcb);
    return 0;
}

void setYield(){
    isYield = YIELD_DONE;
}
void clearYield(){
    isYield = YIELD_NOT_DONE;
}
char getYield(){
    return isYield;
}