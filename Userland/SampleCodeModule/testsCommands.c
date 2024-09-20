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
    test_mm(1,"2500"); 
}

void quit(void){
    sysClearScreen();
    init();
}

void commandNotFound(char* commandNotFound){
    print(commandNotFound); 
    print(": command not found.\n");
}