// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>
#include <memoryManager.h>
#include <defs.h>
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <interrupts.h>
#include <scheduler.h>

extern void *setupStack(entryPoint entryPoint, void *stackBase, int argc, char *argv[]);

Process processes[MAX_PROCESSES];
PID current;

PID initProcesses(void)
{
    current = 1;
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        processes[i].pid = NONPID;
    }
    return 0;
}

int checkPriority(Priority priority)
{
    return priority >= MIN_PRIORITY && priority <= MAX_PRIORITY;
}

int checkName(const char *name)
{
    return name != NULL && strlen(name) <= MAX_NAME_LENGTH;
}

int getFreeProcess()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        if (processes[i].pid == NONPID)
        {
            return i;
        }
    }
    return -1;
}

PID createProcess(const char *name, int argc, char *argv[], Priority priority, entryPoint entryPoint, int foreground)
{
    if (!checkPriority(priority) || argc < 0 || entryPoint == NULL || !checkName(name) || current > MAX_PID)
    {
        return -1;
    }
    void *stackLimit = mallocMM(STACK_SIZE);
    char **args;
    if (stackLimit == NULL || (argc != 0 && (args = mallocMM(argc * sizeof(char *))) == NULL))
    {
        freeMM(stackLimit);
        freeMM(args);
        return -1;
    }

    // Copy args
    for (int i = 0; i < argc; i++)
    {
        int len = strlen(argv[i]);
        if ((args[i] = mallocMM(len + 1)) == NULL)
        {
            for (int j = 0; j < i; j++)
            {
                freeMM(args[j]);
            }
            freeMM(args);
            freeMM(stackLimit);
            return -1;
        }
        memcpy(args[i], argv[i], len);
    }

    PID pid = current++;
    Process *currentProcess;
    int allocatedProcess = getFreeProcess();
    if (allocatedProcess == -1)
    {
        freeMM(stackLimit);
        for (int i = 0; i < argc; i++)
        {
            freeMM(args[i]);
        }
        freeMM(args);
        return -1;
    }

    // Set current process Information
    memcpy(processes[pid].name, name, strlen(name) + 1);
    processes[allocatedProcess].pid = pid;
    processes[allocatedProcess].parentpid = (currentProcess = getCurrentProcess()) == NULL ? 0 : currentProcess->pid;
    processes[allocatedProcess].argc = argc;
    processes[allocatedProcess].argv = args;
    processes[allocatedProcess].priority = priority;
    processes[allocatedProcess].entryPoint = entryPoint;
    processes[allocatedProcess].foreground = foreground;
    processes[allocatedProcess].state = READY;
    processes[allocatedProcess].stackBase = stackLimit + STACK_SIZE;
    processes[allocatedProcess].stackEnd = setupStack(entryPoint, processes[allocatedProcess].stackBase, argc, args);

    schedule(&(processes[allocatedProcess]));
    return pid;
    // TODO: Handle entryPoint return value
}

PID getpid(void)
{
    return getCurrentProcess()->pid;
}
