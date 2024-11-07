// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/modes.h"
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/syscalls.h"
#include "include/lib.h"
#include "include/dateTime.h"
#include "include/colors.h"
#include "include/utils.h"
#include "include/processStructure.h"
#include "include/test_util.h"
#include "include/tests.h"

#define MAX_BUFF_LEN 30

char *dateTimeAux;
int zoomAux, regAux;
int memoryStatus[4];

static char *helpText[] = {"Command information is displayed below:\n\n",
                           "HELP                ->      Shows a description on each available command.\n",
                           "TIME                ->      Shows current time in HH:MM:SS format.\n",
                           "DATE                ->      Shows current date in DD/MM/YY format.\n",
                           "CLEAR               ->      Clears the screen\n",
                           "REGISTERS           ->      Prints registers values. To do this, first you need to save\n",
                           "                            your registers by pressing ALT.\n",
                           "YIELD               ->      Relinquish voluntarily the CPU\n",
                           "PS                  ->      Shows information about all processes\n",
                           "KILL [pid]          ->      Kills the process with the given pid\n",
                           "SUSPEND [pid]       ->      Suspend a process with the given pid.\n",
                           "RESUME [pid]        ->      Resume a process with the given pid.\n",
                           "NICE [pid] [prio]   ->      Change the priority of the process with the given pid to prio.\n",
                           "MEMSTATUS           ->      Prints how many blocks and bytes were allocated \n",
                           "                            and how many bytes are still free.\n",
                           "TESTMM              ->      Test memory manager.\n",
                           "TESTPROC            ->      Test processes creation.\n",
                           "TESTPRIO            ->      Test processes priority.\n",
                           "TESTSYNC            ->      Test syncronization with semaphores.\n",
                           "TESTNOSYNC          ->      Test syncronization without semaphores.\n",
                           "TESTPIPE            ->      Test pipes.\n",
                           "end"};

char *states[5] = {"Ready", "Running", "Blocked", "Dead", "Foreground"};

void help(void)
{
    for (int i = 0; strcasecmp(helpText[i], "end") != 0; i++)
    {
        printColor(helpText[i], YELLOW);
    }
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
               (int32_t)processes[i].pid);
        print(processes[i].name);
        printf(" | ParentPID=%d | Priority=%d | Foreground=%d | State=",
               (int32_t)processes[i].parentpid,
               (int32_t)processes[i].priority,
               (int32_t)processes[i].foreground);
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
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
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

void getMemoryStatus()
{
    sysGetMemStatus(memoryStatus);
    printf("Memoria total= %d B |  ", memoryStatus[3]);
    printf("Memoria ocupada= %d B (%d)|  ", memoryStatus[1], memoryStatus[1] * 100 / memoryStatus[3]);
    printf("Memoria libre= %d B | ", memoryStatus[2]);
    printf("Bloques ocupados= %d\n", memoryStatus[0]);
}

int testProc(int processCount, int foreground)
{
    char count[MAX_BUFF_LEN];
    intToString(processCount, count, 0);
    sysHideCursor();
    print("You are testing processes\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    print("Press 'q' to finish the test\n\n");
    char *argv[] = {count, 0};
    creationParameters params;

    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.name = "test_processes";
    params.argc = 1;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_processes;
    params.foreground = foreground;

    return createProcess(&params);
}

int testPrio(int foreground)
{
    sysHideCursor();
    print("You are testing processes priorities\n");
    print("Below you will see information on running processes\n");
    print("Each process will print its PID an amount of times based on its priority\n\n");
    creationParameters params;
    params.name = "test_prio";
    params.argc = 0;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.argv = NULL;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_prio;
    params.foreground = foreground;

    return createProcess(&params);
}

int testMM(int maxMem, int foreground)
{
    char max[MAX_BUFF_LEN];
    intToString(maxMem, max, 0);
    sysHideCursor();
    print("You are testing memory manager\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {max, 0};
    creationParameters params;
    params.name = "test_mm";
    params.argc = 1;
    params.argv = argv;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_mm;
    params.foreground = foreground;

    return createProcess(&params);
}

int testSync(int n, int foreground)
{
    char start[MAX_BUFF_LEN];
    intToString(n, start, 0);
    sysHideCursor();
    print("You are testing syncronization with semaphores\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {start, "1", 0};
    creationParameters params;
    params.name = "test_sync";
    params.argc = 2;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.entryPoint = (entryPoint)test_sync;
    params.foreground = foreground;

    return createProcess(&params);
}

int testNoSync(int n, int foreground)
{
    char start[MAX_BUFF_LEN];
    intToString(n, start, 0);
    sysHideCursor();
    print("You are testing syncronization without semaphores\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {start, "0", 0};
    creationParameters params;
    params.name = "test_sync";
    params.argc = 2;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.entryPoint = (entryPoint)test_sync;
    params.foreground = foreground;

    return createProcess(&params);
}

int64_t inputP(uint64_t argc, char *argv[])
{

    char c;
    char buffer[100] = {'\0'};
    return 0;
    while (1)
    {
        scanf(buffer, 100);
        if (strcmp(buffer, "exit") == 0)
        {
            return 0;
        }
    }
    return 0;
}

int64_t outputP(uint64_t argc, char *argv[])
{

    char c;
    char buffer[100] = {'\0'};
    while (1)
    {
        scanf(buffer, 100);
        if (strcmp(buffer, "exit") == 0)
        {
            return 0;
        }
    }
    return 0;
}
void dummy() {}
int outputAlone(uint64_t argc, char *argv[])
{

    unsigned char c;
    char buffer[100] = {'\0'};

    while ((c = getchar()) != EOF)
    {
        putchar(c);
    }
    return 0;
}

int testPipe(void)
{
    sysHideCursor();
    print("Testing pipes\n");
    print("Two processes were created, one reads from STDIN, the other writes to STDOUT \n");
    print("this test works like 'input | output'\n");
    print("Type 'exit' and press enter to exit\n");

    //  print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {0};
    int fds[2];
    int pids[2];
    //  sysPipe(fds);
    creationParameters params;
    //  params.name = "inputP";
    //  params.argc =0;
    //  params.argv = argv;
    //  params.priority = 1;
    //   params.fds[0] = STDIN;
    //   params.fds[1] = fds[1];
    //   params.entryPoint = (entryPoint)inputP;
    //   params.foreground = 1;
    // pids[0] = createProcess(&params);
    params.name = "outputP";
    params.argc = 0;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.entryPoint = (entryPoint)outputAlone;
    params.foreground = 1;
    pids[1] = createProcess(&params);
    //   sysWait(pids[0], NULL);
    sysWait(pids[1], NULL);
    dummy();
    return 0;
}
