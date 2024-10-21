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
#include "./include/tests.h"

static char *helpTextTestingArea[] = {"Tests command information is displayed below\n",
                                      "Disclaimer: each test loops forever, you can finish it pressing 'q'\n\n",
                                      "HELP                ->      Shows a description on each available command.\n",
                                      "TestMM              ->      Test memory manager.\n",
                                      "TestProc            ->      Test processes creation.\n",
                                      "TestPrio            ->      Test processes priority.\n",
                                      "q                   ->      Return to shell.\n",
                                      "end"};

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

void testMM(void)
{
    sysHideCursor();
    print("You are testing memory manager\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    print("Press 'q' to finish the test\n\n");
    char *argv[] = {"25000", 0};
    creationParameters params;
    params.name = "test_mm";
    params.argc = 1;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_mm;
    params.foreground = 1;

    int pid = createProcess(&params);
    sysWait(pid, NULL);
    sysShowCursor();
}