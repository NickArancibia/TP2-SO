// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/semaphore.h"
#include "./include/queueADT.h"
#include "./include/memoryManager.h"
#include "string.h"
#include <lib.h>
typedef struct
{
    uint8_t isInUse;
    int value;
    int isAvailable;
    char *name;
    PID openBy[MAX_PROCESSES];
    QueueADT waitingProcess;
} sem;
sem semaphores[MAX_SEMAPHORES];
int totalControl;

int initializeSems()
{
    totalControl = 0;
    for (int i = 0; i < MAX_SEMAPHORES; i++)
    {
        semaphores[i].isAvailable = 1;
        semaphores[i].waitingProcess = newQueue();
        memset(semaphores[i].openBy, -1, sizeof(semaphores[i].openBy));
        semaphores[i].isInUse = 0;
        semaphores[i].name = NULL;
    }
    return 0;
}

int wasOpenBy(sem semaphore, PID pid)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (pid == 1 || semaphore.openBy[i] == pid)
        {
            return 1;
        }
    }
    return -1;
}

int isOpenByEmpty(sem semaphore)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (semaphore.openBy[i] != -1)
        {
            return 0;
        }
    }
    return 1;
}

int setOpenBy(int idx, PID pid)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (semaphores[idx].openBy[i] == -1)
        {
            semaphores[idx].openBy[i] = pid;
            return 1;
        }
    }
    return -1;
}

int removeOpenBy(int idx, PID pid)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (semaphores[idx].openBy[i] == pid)
        {
            semaphores[idx].openBy[i] = -1;
            return 1;
        }
    }
    return -1;
}

int semGet(char *semID)
{
    for (int i = 0; i < MAX_SEMAPHORES; i++)
    {
        if (semaphores[i].isAvailable == 0 && strcmp(semID, semaphores[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}

int getAvailableSem()
{
    for (int i = 0; i < MAX_SEMAPHORES; i++)
    {
        if (semaphores[i].isAvailable)
        {
            return i;
        }
    }
    return -1;
}

int semOpen(char *sem_id, int initialValue)
{
    int idx;

    if (sem_id != NULL && (idx = semGet(sem_id)) != -1)
    {
        setOpenBy(idx, getpid());
        return idx;
    }
    if ((idx = getAvailableSem()) == -1)
    {
        return -1;
    }

    if (sem_id != NULL)
    {
        setOpenBy(idx, getpid());
        int len = strlen(sem_id) + 1;
        semaphores[idx].name = mallocMM(len);
        memcpy(semaphores[idx].name, sem_id, len);
    }
    semaphores[idx].isAvailable = 0;
    semaphores[idx].value = initialValue;
    return idx;
}

int semCreate(int initialValue)
{
    int idx;
    if ((idx = getAvailableSem()) == -1)
    {
        return -1;
    }
    semaphores[idx].isAvailable = 0;
    semaphores[idx].value = initialValue;
    return idx;
}

int semWait(int sem_id)
{

    acquire(&semaphores[sem_id].isInUse);
    if (semaphores[sem_id].name != NULL && !wasOpenBy(semaphores[sem_id], getpid()))
    {
        release(&semaphores[sem_id].isInUse);
        return -1;
    }

    while (semaphores[sem_id].value == 0)
    {
        queue(semaphores[sem_id].waitingProcess, getpid());
        release(&semaphores[sem_id].isInUse);
        blockProcess(getpid());

        acquire(&semaphores[sem_id].isInUse);
    }
    semaphores[sem_id].value--;
    release(&semaphores[sem_id].isInUse);
    return 0;
}

int semPost(int sem_id)
{

    acquire(&semaphores[sem_id].isInUse);
    if (semaphores[sem_id].name != NULL && !wasOpenBy(semaphores[sem_id], getpid()))
    {
        release(&semaphores[sem_id].isInUse);
        return -1;
    }
    semaphores[sem_id].value++;
    if (!isEmpty(semaphores[sem_id].waitingProcess))
    {
        PID pid = dequeue(semaphores[sem_id].waitingProcess);
        unblockProcess(pid);
    }
    release(&semaphores[sem_id].isInUse);
    return 0;
}

int semClose(int sem_id)
{
    if (semaphores[sem_id].isAvailable)
    {
        return -1;
    }
    acquire(&semaphores[sem_id].isInUse);
    if (!isEmpty(semaphores[sem_id].waitingProcess))
    {
        removeOpenBy(sem_id, getpid());
        release(&semaphores[sem_id].isInUse);
        return 0;
    }
    if (semaphores[sem_id].name == NULL || isOpenByEmpty(semaphores[sem_id]))
    {
        memset(semaphores[sem_id].openBy, -1, sizeof(semaphores[sem_id].openBy));
        semaphores[sem_id].isAvailable = 1;
        if (semaphores[sem_id].name != NULL)
        {
            memset(semaphores[sem_id].name, 0, strlen(semaphores[sem_id].name) + 1);
            freeMM(semaphores[sem_id].name);
        }
        release(&semaphores[sem_id].isInUse);
    }
    release(&semaphores[sem_id].isInUse);
    return 0;
}
