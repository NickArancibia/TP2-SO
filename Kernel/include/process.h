#ifndef __PROCESS_H
#define __PROCESS_H

#define MAX_NAME_LENGTH 20
#define MAX_PROCESSES 10
#define MAX_PRIORITY 5
#define MIN_PRIORITY 1
#define DEFAULT_PRIORITY MIN_PRIORITY
#define MAX_PID MAX_PROCESSES
#define NONPID -1
#define INITPID 1

#include "defs.h"
#include <stdint.h>
typedef int (*entryPoint)(int argc, char *argv[]);

typedef struct
{
    int argc;
    char **argv;
    char *name;
    Priority priority;
    entryPoint entryPoint;
    int foreground;
} creationParameters;

typedef struct
{
    char name[MAX_NAME_LENGTH];
    PID pid, parentpid;
    int argc;
    char **argv;
    Priority priority;
    entryPoint entryPoint;
    int foreground;
    ProcessState state;
    uint64_t *stackBase, *stackEnd;
} Process;

PID initProcesses(void);
PID createProcess(creationParameters *params);
PID getpid(void);
PID getppid(void);
Process * getProcess(PID pid);

Process *getProcessesInformation();
void freeProcessesInformation(Process *processesInfo);

#endif