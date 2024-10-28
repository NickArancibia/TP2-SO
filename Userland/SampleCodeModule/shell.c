// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"
#include "include/modes.h"
#include "include/colors.h"
#include "include/syscalls.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/lib.h"
#include "include/test_util.h"

static const char *modes[] = {
    "shell", "idle", "help", "time",
    "date", "clear", "registers", "easteregg", "ps", "yield",
    "kill", "suspend", "resume", "nice", "memstatus", "testmm",
    "testproc", "testprio", "testsync", "testnosync"};

int init()
{
    printColor("Welcome to Shell! Type HELP for command information.\n\n", YELLOW);
    char commandPrompt[32] = {0};
    int separator, returnValue;
    while (IDLE_MODE)
    {
        sysClearKbEntry();
        printColor("$", GREEN);
        print("> ");
        sysShowCursor();
        scanf(commandPrompt, 32);
        separator = divideString(commandPrompt);
        if (strcasecmp(commandPrompt, modes[HELP_MODE]) == SELECTED_MODE)
            help();
        else if (strcasecmp(commandPrompt, modes[CLEAR_MODE]) == SELECTED_MODE)
            clear();
        else if (strcasecmp(commandPrompt, modes[TIME_MODE]) == SELECTED_MODE)
            time();
        else if (strcasecmp(commandPrompt, modes[DATE_MODE]) == SELECTED_MODE)
            date();
        else if (strcasecmp(commandPrompt, modes[EASTEREGG_MODE]) == SELECTED_MODE)
            playEasterEgg();
        else if (strcasecmp(commandPrompt, modes[REGISTERS_MODE]) == SELECTED_MODE)
            registers();
        else if (strcasecmp(commandPrompt, modes[YIELD]) == SELECTED_MODE)
            yield();
        else if (strcasecmp(commandPrompt, modes[PS]) == SELECTED_MODE)
            printProcessesInformation();
        else if (strcasecmp(commandPrompt, modes[MEMORY_STATUS]) == SELECTED_MODE)
            getMemoryStatus();
        else if (strcasecmp(commandPrompt, modes[KILL]) == SELECTED_MODE)
        {
            int toKill = satoi(&commandPrompt[separator + 1]);
            if (toKill == 0)
            {
                printf("Use: kill [pid] (e.g. kill 3)\n");
            }
            else
            {
                if (sysGetPID() == toKill)
                {
                    printColor("Why? :( \n", RED);
                }
                returnValue = sysKill(toKill);
                if (returnValue == -1)
                {
                    printf("Invalid PID\n");
                }
            }
        }
        else if (strcasecmp(commandPrompt, modes[SUSPEND]) == SELECTED_MODE)
        {
            int toSuspend = satoi(&commandPrompt[separator + 1]);
            if (toSuspend == 0)
            {
                printf("Use: suspend [pid] (e.g. suspend 3)\n");
            }
            else
            {
                returnValue = sysSuspendProcess(toSuspend);
                if (returnValue == -1)
                {
                    printf("Invalid PID\n");
                }
            }
        }
        else if (strcasecmp(commandPrompt, modes[RESUME]) == SELECTED_MODE)
        {
            int toResume = satoi(&commandPrompt[separator + 1]);
            if (toResume == 0)
            {
                printf("Use: resume [pid] (e.g. resume 3)\n");
            }
            else
            {
                returnValue = sysResumeProcess(toResume);
                if (returnValue == -1)
                {
                    printf("Invalid PID\n");
                }
            }
        }
        else if (strcasecmp(commandPrompt, modes[NICE]) == SELECTED_MODE)
        {
            int newSeparator = divideString(&commandPrompt[separator + 1]);
            if (newSeparator == -1)
            {
                printf("Use: nice [pid] [priority] (e.g. nice 3 2)\n");
            }
            else
            {
                int toNice = satoi(&commandPrompt[separator + 1]);
                int priority = satoi(&commandPrompt[separator + newSeparator + 2]);
                if (toNice == 0 || priority == 0)
                {
                    printf("Use: nice [pid] [priority] (e.g. nice 3 2)\n");
                }
                else
                {
                    returnValue = sysNice(toNice, priority);
                    if (returnValue == -1)
                    {
                        printf("Invalid PID or priority\n");
                    }
                }
            }
        }
        else if (strcasecmp(commandPrompt, modes[TEST_MM]) == SELECTED_MODE)
        {
            char isForeground = commandPrompt[separator + 1];
            int pid = testMM();
            if (isForeground == -1 || isForeground != '&')
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else if (strcasecmp(commandPrompt, modes[TEST_PROCS]) == SELECTED_MODE)
        {
            char isForeground = commandPrompt[separator + 1];
            int pid = testProc();
            if (isForeground == -1 || isForeground != '&')
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else if (strcasecmp(commandPrompt, modes[TEST_PRIO]) == SELECTED_MODE)
        {
            char isForeground = commandPrompt[separator + 1];
            int pid = testPrio();
            if (isForeground == -1 || isForeground != '&')
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else if (strcasecmp(commandPrompt, modes[TEST_SYNC]) == SELECTED_MODE)
        {
            char isForeground = commandPrompt[separator + 1];
            int pid = testSync();

            if (isForeground == -1 || isForeground != '&')
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else if (strcasecmp(commandPrompt, modes[TEST_NO_SYNC]) == SELECTED_MODE)
        {
            char isForeground = commandPrompt[separator + 1];
            int pid = testNoSync();

            if (isForeground == -1 || isForeground != '&')
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else
            notFound(commandPrompt);
    }
}