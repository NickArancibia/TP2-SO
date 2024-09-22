// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/testsCommands.h"
#include "./include/colors.h"
#include "./include/testingArea.h"
#include "./include/stdio.h"
#include "./include/string.h"
#include "./include/syscalls.h"
#include "./include/shell.h"

void helpCommands (void) {
    for(int i=0; strcasecmp(helpTextTestingArea[i], "end")!=0; i++){
        printColor(helpTextTestingArea[i], YELLOW);
    }
}

void testMM(void){
    char * argv[]={"25000", 0};
    test_mm(1, argv); 
}

void quit(void){
    sysClearScreen();
    init();
}

void commandNotFound(char* commandNotFound){
    print(commandNotFound); 
    print(": command not found.\n");
}