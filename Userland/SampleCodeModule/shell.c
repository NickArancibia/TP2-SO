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

int init()
{
    printColor("Welcome to Shell! Type HELP for command information.\n\n", YELLOW);
    while (IDLE_MODE)
    {
        char commandPrompt[64] = {0};
        sysClearKbEntry();
        printColor("$", GREEN);
        print("> ");
        sysShowCursor();
        scanf(commandPrompt, 64);
        parseConsolePrompt(commandPrompt);
    }
}
