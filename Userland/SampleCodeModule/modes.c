// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/modes.h"
#include "include/shell.h"
#include "include/eliminatorGame.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/syscalls.h"
#include "include/lib.h"
#include "include/exceptions.h"
#include "include/dateTime.h"
#include "include/colors.h"
#include "include/utils.h"
#include "include/testingArea.h"
#include "include/processStructure.h"
#include "include/test_util.h"

char *dateTimeAux;
int zoomAux, regAux;
int memoryStatus[3];
int64_t test_processes(uint64_t argc, char *argv[]);
void endless_loop();
int iteration;

typedef struct P_rq
{
    int32_t pid;
    ProcessState state;
} p_rq;

static char *helpText[] = {"Command information is displayed below:\n\n",
                           "HELP                ->      Shows a description on each available command.\n",
                           "DIVBYZERO           ->      Shows handling in case of division by zero.\n",
                           "INVALIDOPCODE       ->      Shows handling in case of an invalid operation code.\n",
                           "ZOOMIN              ->      Enlarges text size on screen. In case maximum size is reached,\n",
                           "                            it is properly indicated without making any changes.\n",
                           "                            Can be alternatively activated with CTRL + i\n",
                           "ZOOMOUT             ->      Reduces text size on screen. In case minimum size is reached,\n",
                           "                            it is properly indicated without making any changes.\n",
                           "                            Can be alternatively activated with CTRL + o\n",
                           "TIME                ->      Shows current time in HH:MM:SS format.\n",
                           "DATE                ->      Shows current date in DD/MM/YY format.\n",
                           "ELIMINATOR          ->      Opens ELIMINATOR game.\n",
                           "CLEAR               ->      Clears the screen\n",
                           "REGISTERS           ->      Prints registers values. To do this, first you need to save\n",
                           "                            your registers by pressing ALT.\n",
                           "TESTING             ->      Start testing area\n",
                           "YIELD               ->      Relinquish voluntarily the CPU\n",
                           "PS                  ->      Shows information about all processes\n",
                           "KILL [pid]          ->      Kills the process with the given pid\n",
                           "SUSPEND [pid]       ->      Suspend a process with the given pid.\n",
                           "RESUME [pid]        ->      Resume a process with the given pid.\n",
                           "NICE [pid] [prio]   ->      Change the priority of the process with the given pid to prio.\n",
                           "MEMSTATUS           ->      Prints how many blocks and bytes were allocated \n",
                           "                            and how many bytes are still free.\n",
                           "end"};

char *states[5] = {"Ready", "Running", "Blocked", "Dead", "Foreground"};

void testingArea()
{
    initTestingArea();
}

void help(void)
{
    for (int i = 0; strcasecmp(helpText[i], "end") != 0; i++)
    {
        printColor(helpText[i], YELLOW);
    }
}

void eliminator()
{
    sysHideCursor();
    print("\nLoading eliminator...");
    sysSleep(2, 0);
    eliminatorGame();
}

void clear(void)
{
    sysClearScreen();
}

void time(void)
{
    printTime();
}

void date(void)
{
    printDate();
}
void dummy(){}
void zoomin()
{
    zoomAux = incTextSize();
    if (zoomAux)
        print("Maximum size reached.\n");
}

void zoomout()
{
    zoomAux = decTextSize();
    if (zoomAux)
        print("Minimum size reached.\n");
}

void divByZero()
{
    divZero();
}

void invalidOp()
{
    invalidOpcode();
}

void registers()
{
    regAux = sysPrintRegs();
    if (regAux)
    {
        print("You need to save registers first by pressing ALT\n");
    }
}

void yield()
{
    sysYield();
}

void notFound(char *commandNotFound)
{
    print(commandNotFound);
    print(": command not found.\n");
}

void playEasterEgg()
{
    printColor("I'm", RED);
    printColor(" an", YELLOW);
    printColor(" eas", GREEN);
    printColor("ter ", BLUE);
    printColor("egg", WHITE);
    printColor("!\n", YELLOW);

    sysHideCursor();
    sysPrintCursor();

    sysBeepSound(220, DO);
    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);

    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, DO);
    for (int i = 0; i < 2; i++)
    {
        sysBeepSound(220, SOL_SOST);
        sysBeepSound(220, FA);
    }

    sysBeepSound(220, DO);
    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);

    sysBeepSound(110, FA);
    sysBeepSound(165, FA_SOST);
    sysBeepSound(110, SOL);
    sysBeepSound(165, SOL_SOST);
    sysBeepSound(110, LA);
    sysBeepSound(165, SI);
    sysBeepSound(110, DO_PRIMA);

    sysShowCursor();
    sysPrintCursor();
}

void ps()
{
    Process *processes = sysGetPS();
    for (int i = 0; processes[i].pid != -1; i++)
    {
        printf("PID=%d | Name=",
               processes[i].pid);
        print(processes[i].name);
        printf(" | ParentPID=%d | Priority=%d | Foreground=%d | State=",
               processes[i].parentpid,
               processes[i].priority,
               processes[i].foreground);
        print(states[processes[i].state]);
        printf(" | StackBase=0x");
        print(hexToString((uint64_t)processes[i].stackBase));
        printf(" | StackEnd=0x");
        print(hexToString((uint64_t)processes[i].stackEnd));
        printf("\n");
    }
    sysFreePS(processes);
}

void printProcessesInformation()
{
    PID pid;
    creationParameters params;
    params.name = "ps";
    params.argc = 0;
    params.argv = NULL;
    params.priority = 1;
    params.entryPoint = (entryPoint)ps;
    params.foreground = 1;
    pid = createProcess(&params);
    sysWait(pid, NULL);
    return;
}

void getMemoryStatus(){
    sysGetMemStatus(memoryStatus);
    printf("Bloques ocupados= %d | ",memoryStatus[0]);
    printf("Bytes ocupados= %d |  ",memoryStatus[1]);
    printf("Bytes libres= %d\n",memoryStatus[2]);
}

void testProc(void)
{
    sysHideCursor();
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
        printf("Cr %d\n",++iteration);

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
     //    for (int i = 0; i < 9000000; i++)
     //        ;
     //    printColor("OK!", GREEN);
        
     //   dummy();
       
    }
}

void endless_loop()
{
    while (1)
        ;
}
