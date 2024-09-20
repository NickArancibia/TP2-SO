#include "include/stdlib.h"
#include "include/syscalls.h"


void * malloc(){
    return sysMalloc();
}

// deallocate the block pointed by ptrBlock
void free(void * ptrBlock){
    sysFree(ptrBlock);
}