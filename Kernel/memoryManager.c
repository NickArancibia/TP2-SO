#include <memoryManager.h>
#include "./include/defs.h"

#define BLOCK_COUNT 250

void * start;
int size;

void *free_ptrs[BLOCK_COUNT];
int current;

int memSize, blockSize;

void initializeMemoryMM(void * memoryStart, int memorySize){
        start = memoryStart;
        memSize=memorySize;
        blockSize=memorySize/BLOCK_COUNT;
        for( int i = 0 ; i < BLOCK_COUNT ; i++){
            free_ptrs[i] = memoryStart + blockSize * i;
        }
        current = 0;
}

void *mallocMM(int size){
    if(size <= blockSize && current < BLOCK_COUNT){
            return free_ptrs[current++];
    }
    return NULL;
}

int freeMM(void * memorySegment){
    free_ptrs[--current] = memorySegment;        
    return 0;
}