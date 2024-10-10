// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/shell.h"
#include "include/modes.h"
#include "include/colors.h"
#include "include/syscalls.h"
#include "include/stdio.h"
#include "include/string.h"

static const char *modes[] = {
    "shell", "idle", "help", "divbyzero", "invalidopcode", "zoomin", "zoomout", "time", "date", "eliminator", "clear", "registers", "easteregg", "testing","yield"};

void testBlock(){
    sysSuspendProcess(2);
    for(int i=0; i<200; i++){
        printf("Testing block %d\n", i);
    }
    sysClearScreen();
    sysResumeProcess(2);
}

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
        else if (strcasecmp(commandPrompt, modes[YIELD]) == SELECTED_MODE)
            yield();
        // Used to test pid and getppid vvv
        else if (strcasecmp(commandPrompt, "pid") == SELECTED_MODE)
        {
            printf("PID: %d\n", sysGetPID());
            printf("Parent PID: %d\n", sysGetParentPID());
        }
        else if(strcasecmp(commandPrompt, "create") == SELECTED_MODE){
            sysCreateProcess("block", 0, NULL, 1, testBlock, 1);
            for(int i=0; i<9000000; i++);
            printf("Process created\n");
        }
        else if(strcasecmp(commandPrompt, "suspend") == SELECTED_MODE){
            sysSuspendProcess(3);
        }
        else if(strcasecmp(commandPrompt, "resume") == SELECTED_MODE){
            sysResumeProcess(3);
        }
        else
            notFound(commandPrompt);
    }
}