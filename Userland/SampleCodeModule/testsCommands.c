// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/testsCommands.h"
#include "./include/colors.h"
#include "./include/testingArea.h"
#include "./include/stdio.h"
#include "./include/string.h"
#include "./include/syscalls.h"
#include "./include/shell.h"
#include "./include/processStructure.h"
#include "./include/test_util.h"
#include "./include/lib.h"
#define MINOR_WAIT 900
#define WAIT 10000000

#define TOTAL_PROCESSES 3
#define LOWEST 1
#define MEDIUM 3
#define HIGHEST 5

static char *helpTextTestingArea[] = {"Tests command information is displayed below\n",
                                      "Disclaimer: each test loops forever, you can finish it pressing 'q'\n\n",
                                      "HELP                ->      Shows a description on each available command.\n",
                                      "TestMM              ->      Test memory manager.\n",
                                      "TestProc            ->      Test processes creation.\n",
                                      "TestPrio            ->      Test processes priority.\n",
                                      "q                   ->      Return to shell.\n",
                                      "end"};

int64_t test_processes(uint64_t argc, char *argv[]);
void test_prio();
void endless_loop();
void endless_loop_print(uint64_t wait);
int iteration;
int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

typedef struct P_rq
{
    int32_t pid;
    ProcessState state;
} p_rq;

void helpCommands(void)
{
    for (int i = 0; strcasecmp(helpTextTestingArea[i], "end") != 0; i++)
    {
        printColor(helpTextTestingArea[i], YELLOW);
    }
}

void quit(void)
{
    sysClearScreen();
    init();
}

void commandNotFound(char *commandNotFound)
{
    print(commandNotFound);
    print(": command not found.\n");
}

int checkProcsExit()
{
    if (getchar() == 'q')
    {
        return 1;
    }
    return 0;
}

void testProc(void)
{
    sysHideCursor();
    print("You are testing processes\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    print("Press 'q' to finish the test\n\n");
    char *argv[] = {"6", 0};
    creationParameters params;
    params.name = "test_processes";
    params.argc = 1;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_processes;
    params.foreground = 1;

    int pid = createProcess(&params);
    sysWait(pid, NULL);
    sysShowCursor();
}

int64_t test_processes(uint64_t argc, char *argv[])
{
    int iteration = 0;
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};
    creationParameters params;
    params.name = "endless_loop";
    params.argc = 0;
    params.argv = argvAux;
    params.priority = 1;
    params.entryPoint = (entryPoint)endless_loop;
    params.foreground = 1;

    if (argc != 1)
        return -1;

    if ((max_processes = satoi(argv[0])) <= 0)
        return -1;

    p_rq p_rqs[max_processes];
    while (1)
    {
        if (iteration)
        {
            print("\b\b\b");
        }
        printColor("OK!", GREEN);
        iteration = 1;
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++)
        {
            p_rqs[rq].pid = createProcess(&params);

            if (p_rqs[rq].pid == -1)
            {
                printf("test_processes: ERROR creating process\n");
                return -1;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {

            for (rq = 0; rq < max_processes; rq++)
            {
                action = GetUniform(100) % 2;

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        if (sysKill(p_rqs[rq].pid) == -1)
                        {
                            printf("test_processes: ERROR killing process\n");
                            return -1;
                        }
                        p_rqs[rq].state = DEAD;
                        alive--;
                    }
                    break;
                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (sysSuspendProcess(p_rqs[rq].pid) == -1)
                        {
                            printf("test_processes: ERROR blocking process\n");
                            return -1;
                        }
                        p_rqs[rq].state = BLOCKED;
                    }
                    break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < max_processes; rq++)
                if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2)
                {
                    if (sysResumeProcess(p_rqs[rq].pid) == -1)
                    {
                        printf("test_processes: ERROR unblocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
        if (checkProcsExit())
        {
            printf("\n");
            return 0;
        }
    }
}

void testPrio()
{
    sysHideCursor();
    print("You are testing processes priorities\n");
    print("Below you will see information on running processes\n");
    print("Each process will print its PID an amount of times based on its priority\n\n");
    PID pid;
    creationParameters params;
    params.name = "test_prio";
    params.argc = 0;
    params.argv = NULL;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_prio;
    params.foreground = 1;
    sysSleep(1, 0);
    pid = createProcess(&params);
    sysWait(pid, NULL);
    sysShowCursor();
}

void test_prio()
{
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;
    creationParameters params;
    params.name = "endless_loop_print";
    params.argc = 0;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)endless_loop_print;
    params.foreground = 1;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = createProcess(&params);

    bussy_wait(WAIT);
    printf("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysNice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printf("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysSuspendProcess(pids[i]);

    printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysNice(pids[i], MEDIUM);

    printf("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysResumeProcess(pids[i]);

    bussy_wait(WAIT);
    printf("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        sysKill(pids[i]);
}

void endless_loop_print(uint64_t wait)
{
    int64_t pid = sysGetPID();

    while (1)
    {
        printf("%d ", pid);
        bussy_wait(MINOR_WAIT);
    }
}

void endless_loop()
{
    while (1)
        ;
}