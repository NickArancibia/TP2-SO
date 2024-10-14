// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/testingArea.h"
#include "./include/testsCommands.h"
#include "./include/colors.h"
#include "./include/stdio.h"
#include "./include/string.h"
#include "./include/syscalls.h"
#include "./include/shell.h"

static const char *commands[] = {
    "", "", "help", "testMM", "testProc", "testPrio", "q"};

void initTestingArea()
{
    sysClearScreen();
    printColor("Welcome to Testing area! Type HELP for command information or 'q' for exit.\n\n", YELLOW);
    char commandPrompt[32] = {0};
    while (IDLE_MODE)
    {
        sysClearKbEntry();
        printColor("$", GREEN);
        print("> ");
        sysShowCursor();
        scanf(commandPrompt, 32);
        if (strcasecmp(commandPrompt, commands[HELP]) == SELECTED_MODE)
            helpCommands();
        else if (strcasecmp(commandPrompt, commands[TEST_MM]) == SELECTED_MODE)
            testMM();
        else if (strcasecmp(commandPrompt, commands[PROCS]) == SELECTED_MODE)
            testProc();
        else if (strcasecmp(commandPrompt, commands[PRIO]) == SELECTED_MODE)
            testPrio();
        else if (strcasecmp(commandPrompt, commands[EXIT]) == SELECTED_MODE)
            quit();
        else
            commandNotFound(commandPrompt);
    }
}