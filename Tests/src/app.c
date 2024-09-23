#include "../include/app.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include "../include/testMM.h"
int main() {
    char startMem[MEMORY];

    initializeMemoryMM((void*)startMem,MEMORY);
    char buff[20];
    sprintf(buff, "%d", FREE_MEM);
    char * argv[]={buff, 0};
    test_mm(1,argv);
    return 0;
}
