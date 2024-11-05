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
    }
    return 0;
}

int wasOpenBy(sem semaphore, PID pid)
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (semaphore.openBy[i] == pid)
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

    if ((idx = semGet(sem_id)) != -1)
    {
        setOpenBy(idx, getpid());
        return 0;
    }
    if ((idx = getAvailableSem()) == -1)
    {
        return -1;
    }

    setOpenBy(idx, getpid());
    int len = strlen(sem_id) + 1;
    semaphores[idx].name = mallocMM(len);
    memcpy(semaphores[idx].name, sem_id, len);
    semaphores[idx].isAvailable = 0;
    semaphores[idx].value = initialValue;
    return 0;
}

int semWait(char *sem_id)
{
    int idx;
    if ((idx = semGet(sem_id)) == -1 || wasOpenBy(semaphores[idx], getpid()) == -1)
    {
        return -1;
    }
    acquire(&semaphores[idx].isInUse);
    while (semaphores[idx].value == 0)
    {
        queue(semaphores[idx].waitingProcess, getpid());
        release(&semaphores[idx].isInUse);
        blockProcess(getpid());

        acquire(&semaphores[idx].isInUse);
    }
    semaphores[idx].value--;
    release(&semaphores[idx].isInUse);
    return 0;
}

int semPost(char *sem_id)
{
    int idx;
    if ((idx = semGet(sem_id)) == -1 || wasOpenBy(semaphores[idx], getpid()) == -1)
    {
        return -1;
    }
    acquire(&semaphores[idx].isInUse);
    semaphores[idx].value++;
    if (!isEmpty(semaphores[idx].waitingProcess))
    {
        PID pid = dequeue(semaphores[idx].waitingProcess);
        unblockProcess(pid);
    }
    release(&semaphores[idx].isInUse);
    return 0;
}

int semClose(char *sem_id)
{
    int idx;
    if ((idx = semGet(sem_id)) == -1)
    {
        return 1;
    }
    acquire(&semaphores[idx].isInUse);
    if (!isEmpty(semaphores[idx].waitingProcess) && !isOpenByEmpty(semaphores[idx]))
    {
        removeOpenBy(idx, getpid());
        release(&semaphores[idx].isInUse);
        return 0;
    }

    memset(semaphores[idx].openBy, -1, sizeof(semaphores[idx].openBy));
    semaphores[idx].isAvailable = 1;
    memset(semaphores[idx].name, 0, strlen(semaphores[idx].name) + 1);
    freeMM(semaphores[idx].name);
    release(&semaphores[idx].isInUse);
    //  freeQueue(semaphores[idx].waitingProcess);
    return 0;
}