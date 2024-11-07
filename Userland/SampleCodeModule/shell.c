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
#include "include/programs.h"

#define BUFLEN 32
#define MAXARGC 8

char *token = NULL;
char buffer[BUFLEN] = {0};
char *argv[MAXARGC] = {0};
static const char *modes[] = {
    "shell", "idle", "help", "time",
    "date", "clear", "registers", "easteregg", "ps", "yield",
    "kill", "suspend", "resume", "nice", "memstatus", "testmm",
    "testproc", "testprio", "testsync", "testnosync", "testpipe",
    "loop", "phylo"};

static void getNextToken();

static void getNextToken();

int init()
{
    printColor("Welcome to Shell! Type HELP for command information.\n\n", YELLOW);
    char commandPrompt[32] = {0};
    int returnValue;
    creationParameters params;
    while (IDLE_MODE)
    {
        sysClearKbEntry();
        printColor("$", GREEN);
        print("> ");
        sysShowCursor();
        scanf(commandPrompt, 32);
        token = strtok(commandPrompt, " ");
        if (token == NULL)
            continue;

        if (strcasecmp(token, modes[HELP_MODE]) == SELECTED_MODE)
            help();
        else if (strcasecmp(token, modes[CLEAR_MODE]) == SELECTED_MODE)
            clear();
        else if (strcasecmp(commandPrompt, modes[TEST_PIPES]) == SELECTED_MODE)
        {
            testPipe();
            printf("\n");
        }
        else if (strcasecmp(commandPrompt, modes[TIME_MODE]) == SELECTED_MODE)
            time();
        else if (strcasecmp(token, modes[DATE_MODE]) == SELECTED_MODE)
            date();
        else if (strcasecmp(token, modes[EASTEREGG_MODE]) == SELECTED_MODE)
            playEasterEgg();
        else if (strcasecmp(token, modes[REGISTERS_MODE]) == SELECTED_MODE)
            registers();
        else if (strcasecmp(token, modes[YIELD]) == SELECTED_MODE)
            yield();
        else if (strcasecmp(token, modes[PS]) == SELECTED_MODE)
            printProcessesInformation();
        else if (strcasecmp(token, modes[MEMORY_STATUS]) == SELECTED_MODE)
            getMemoryStatus();
        else if (strcasecmp(token, modes[KILL]) == SELECTED_MODE)
        {
            getNextToken();
            int toKill = satoi(token);
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
        else if (strcasecmp(token, modes[SUSPEND]) == SELECTED_MODE)
        {
            getNextToken();
            int toSuspend = satoi(token);
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
        else if (strcasecmp(token, modes[RESUME]) == SELECTED_MODE)
        {
            getNextToken();
            int toResume = satoi(token);
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
        else if (strcasecmp(token, modes[NICE]) == SELECTED_MODE)
        {
            getNextToken();
            int toNice = satoi(token);
            getNextToken();
            int priority = satoi(token);
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
        else if (strcasecmp(token, modes[TEST_MM]) == SELECTED_MODE)
        {
            getNextToken();
            int maxMem = satoi(token);
            if (maxMem == 0)
            {
                printf("Use: testmm [maxMem] (e.g. testmm 25000)\n");
            }
            else
            {
                getNextToken();
                char isForeground = (token == NULL || strcasecmp(token, "&") != 0);
                int pid = testMM(maxMem, isForeground);
                if (isForeground)
                    sysWait(pid, NULL);
                sysShowCursor();
                printf("\n");
            }
        }
        else if (strcasecmp(token, modes[TEST_PROCS]) == SELECTED_MODE)
        {
            getNextToken();
            int processes = satoi(token);
            if (processes == 0)
            {
                printf("Use: testprocs [processes] (e.g. testprocs 3)\n");
            }
            else
            {
                getNextToken();
                char isForeground = (token == NULL || strcasecmp(token, "&") != 0);
                int pid = testProc(processes, isForeground);
                if (isForeground)
                    sysWait(pid, NULL);
                sysShowCursor();
                printf("\n");
            }
        }
        else if (strcasecmp(token, modes[TEST_PRIO]) == SELECTED_MODE)
        {
            getNextToken();
            char isForeground = (token == NULL || strcasecmp(token, "&") != 0);
            int pid = testPrio(isForeground);
            if (isForeground)
                sysWait(pid, NULL);
            sysShowCursor();
            printf("\n");
        }
        else if (strcasecmp(token, modes[TEST_SYNC]) == SELECTED_MODE)
        {
            getNextToken();
            int n = satoi(token);
            if (n == 0)
            {
                printf("Use: testsync [n] (e.g. testsync 3)\n");
            }
            else
            {
                getNextToken();
                char isForeground = (token == NULL || strcasecmp(token, "&") != 0);
                int pid = testSync(n, isForeground);

                if (isForeground)
                    sysWait(pid, NULL);
                sysShowCursor();
                printf("\n");
            }
        }
        else if (strcasecmp(token, modes[TEST_NO_SYNC]) == SELECTED_MODE)
        {
            getNextToken();
            int n = satoi(token);
            if (n == 0)
            {
                printf("Use: testsync [n] (e.g. testsync 3)\n");
            }
            else
            {
                getNextToken();
                char isForeground = (token == NULL || strcasecmp(token, "&") != 0);
                int pid = testNoSync(n, isForeground);

                if (isForeground)
                    sysWait(pid, NULL);
                sysShowCursor();
                printf("\n");
            }
        }
        else if (strcasecmp(token, modes[LOOP]) == SELECTED_MODE)
        {
            getNextToken();
            if (token == NULL)
            {
                printf("Usage: loop <seconds to sleep>\n");
                continue;
            }
            strcpy(buffer, token);
            argv[0] = buffer;
            argv[1] = NULL;
            params.name = "loop";
            params.priority = 1;
            params.argc = 1;
            params.argv = argv;
            params.fds[0] = STDIN;
            params.fds[1] = STDOUT;
            params.entryPoint = (entryPoint)loop;

            getNextToken();
            params.foreground = (token == NULL || strcasecmp(token, "&") != 0);
            programDispatcher(&params);
        }

        else if (strcasecmp(token, modes[PHYLO]) == SELECTED_MODE)
        {
            getNextToken();
            if (token == NULL)
            {
                printf("Usage: phylo <ammount of philosophers>\n");
                continue;
            }
            strcpy(buffer, token);
            argv[0] = buffer;
            argv[1] = NULL;
            params.name = "phylo";
            params.priority = 1;
            params.argc = 1;
            params.argv = argv;
            params.fds[0] = STDIN;
            params.fds[1] = STDOUT;
            params.entryPoint = (entryPoint)phylo;

            getNextToken();
            params.foreground = (token == NULL || strcasecmp(token, "&") != 0);
            programDispatcher(&params);
        }
        else
            notFound(commandPrompt);
    }
}

static void getNextToken()
{
    token = strtok(NULL, " ");
}