#include <process.h>
#include <memoryManager.h>
#include <defs.h>
#include <stdint.h>
#include <string.h>
#include <lib.h>

extern void *setupStack(void *entryPoint, void *stackBase, int argc, char *argv[]);

typedef struct
{
    char name[MAX_NAME_LENGTH];
    PID pid, parentpid;
    int argc;
    char **argv;
    Priority priority;
    void *entryPoint;
    int foreground;
    ProcessState state;
    uint64_t *stackBase, *stackEnd;
} Process;

Process processes[MAX_PROCESSES];
PID current;

PID initProcesses(void)
{
    current = 1;
    return createProcess("init", 0, NULL, DEFAULT_PRIORITY, NULL, 1);
}

int checkPriority(Priority priority)
{
    return priority >= MIN_PRIORITY && priority <= MAX_PRIORITY;
}

int checkName(const char *name)
{
    return name != NULL && strlen(name) <= MAX_NAME_LENGTH;
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

    // Set current process Information
    processes[pid].pid = pid;
    processes[pid].parentpid = 0; /* TODO: GetcurrentProcessPID */
    processes[pid].argc = argc;
    processes[pid].argv = args;
    processes[pid].priority = priority;
    processes[pid].entryPoint = entryPoint;
    processes[pid].foreground = foreground;
    processes[pid].state = READY;
    processes[pid].stackBase = stackLimit + STACK_SIZE;
    processes[pid].stackEnd = stackLimit;

    setupStack(entryPoint, processes[pid].stackBase, argc, args);
    return pid;
    // TODO: Handle entryPoint return value
}
