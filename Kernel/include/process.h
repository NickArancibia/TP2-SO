#ifndef __PROCESS_H
#define __PROCESS_H

#define MAX_NAME_LENGTH 20
#define MAX_PROCESSES 10
#define MAX_PRIORITY 5
#define MIN_PRIORITY 1
#define DEFAULT_PRIORITY MIN_PRIORITY
#define MAX_PID MAX_PROCESSES

#include <defs.h>

typedef int (*entryPoint)(int argc, char *argv[]);

PID initProcesses(void);
PID createProcess(const char *name, int argc, char *argv[], Priority priority, entryPoint entryPoint, int foreground);

#endif