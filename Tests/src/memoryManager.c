
#include "../include/memoryManager.h"
#include <stddef.h>



memHeader * firstBlock;
int memSize, blockSize;

void splitBlock(memHeader * block, int size);

void initializeMemoryMM(void * memoryStart, int memorySize){
   // memHeader * firstBlock;
    firstBlock = (memHeader *) memoryStart;
    int a = sizeof(memHeader);
    firstBlock->size = memorySize - a;
    firstBlock->isFree = 1;
    firstBlock->next = NULL;
    firstBlock->prev = NULL;
}

void *mallocMM(int size){
    if(size <= 0){
        return NULL;
    }
    // curr;
   memHeader * curr = firstBlock;
    while(curr != NULL && !(curr->isFree && curr->size >= size)){
        curr = curr->next;
    }
    if(curr == NULL){
        return NULL;
    }
    if(curr->size == size){
        curr->isFree = 0;
        return (void *) ((void *)curr + sizeof(memHeader));
    }
    if(curr->size > size){
        splitBlock(curr, size);
        return (void *) (((void*)curr) + sizeof(memHeader));
    }
    return NULL;
}

int freeMM(void * memorySegment){
    if(memorySegment == NULL)
        return 0;
   memHeader *  curr = (memHeader*)((void *) (memorySegment - sizeof(memHeader)));
    curr->isFree=1;
    if(curr->prev != NULL && curr->prev->isFree){
        curr->prev->size += (curr->size + sizeof(memHeader));
        curr->prev->next = curr->next;
        if(curr->next != NULL){
            curr->next->prev = curr->prev;
        }
        curr = curr->prev;
    }
    if(curr->next != NULL && curr->next->isFree){
        curr->size += (curr->next->size + sizeof(memHeader));
        curr->next = curr->next->next;
        if(curr->next != NULL){
            curr->next->prev = curr;
        }
    }
    return 0;
}

void splitBlock(memHeader * block, int size){

  //  newBlock;
 memHeader *   newBlock = (memHeader *) ((void *) block + size + sizeof(memHeader));
    newBlock->size = block->size - size - sizeof(memHeader);
    newBlock->isFree = 1;
    newBlock->next = block->next;
    newBlock->prev = block;
    block->size = size;
    block->isFree = 0;
    block->next = newBlock;
}


