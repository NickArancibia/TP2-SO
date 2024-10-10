// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"
#include "include/modes.h"
#include "include/colors.h"
#include "include/syscalls.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/lib.h"

static const char *modes[] = {
    "shell", "idle", "help", "divbyzero", "invalidopcode", "zoomin", "zoomout", "time", "date", "eliminator", "clear", "registers", "easteregg", "testing", "ps"};

creationParameters params;

int init()
{
    printColor("Welcome to Shell! Type HELP for command information.\n\n", YELLOW);
    char commandPrompt[32] = {0};
    while (IDLE_MODE)
    {
        sysClearKbEntry();
        printColor("$", GREEN);
        print("> ");
        sysShowCursor();
        scanf(commandPrompt, 32);
        if (strcasecmp(commandPrompt, modes[HELP_MODE]) == SELECTED_MODE)
            help();
        else if (strcasecmp(commandPrompt, modes[ELIMINATOR_MODE]) == SELECTED_MODE)
            eliminator();
        else if (strcasecmp(commandPrompt, modes[CLEAR_MODE]) == SELECTED_MODE)
            clear();
        else if (strcasecmp(commandPrompt, modes[TIME_MODE]) == SELECTED_MODE)
            time();
        else if (strcasecmp(commandPrompt, modes[DATE_MODE]) == SELECTED_MODE)
            date();
        else if (strcasecmp(commandPrompt, modes[EASTEREGG_MODE]) == SELECTED_MODE)
            playEasterEgg();
        else if (strcasecmp(commandPrompt, modes[ZOOMIN_MODE]) == SELECTED_MODE)
            zoomin();
        else if (strcasecmp(commandPrompt, modes[ZOOMOUT_MODE]) == SELECTED_MODE)
            zoomout();
        else if (strcasecmp(commandPrompt, modes[DIVBYZERO_MODE]) == SELECTED_MODE)
            divByZero();
        else if (strcasecmp(commandPrompt, modes[INVALIDOPCODE_MODE]) == SELECTED_MODE)
            invalidOp();
        else if (strcasecmp(commandPrompt, modes[REGISTERS_MODE]) == SELECTED_MODE)
            registers();
        else if (strcasecmp(commandPrompt, modes[TESTING_AREA]) == SELECTED_MODE)
            testingArea();
        else if (strcasecmp(commandPrompt, modes[PS]) == SELECTED_MODE)
            printProcessesInformation();
        else if (strcasecmp(commandPrompt, "create") == SELECTED_MODE)
        {
            char *argv[] = {"arg1", "arg2", "arg3"};
            params.name = "test";
            params.argc = 3;
            params.argv = argv;
            params.priority = 1;
            params.entryPoint = NULL;
            params.foreground = 1;
            createProcess(&params);
        }
        else
            notFound(commandPrompt);
    }
}