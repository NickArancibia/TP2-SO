// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include "./include/defs.h"

#define BLOCK_COUNT 250
#define ALIGNMENT 8

// Rounds up to the nearest multiple of the word size (8 bytes)
// If size is already a multiple of the word size, the bitwise operation keeps it unchanged
// Otherwise, it rounds up the size to the next multiple of the word size by adding (ALIGNMENT - 1)
// and then clearing the lower bits using a bitwise operation with ~(ALIGNMENT - 1)
//                    size   + 7 (111b)         & ~(111b)"
//                                              &   (000b)
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define SIZE_MEM_HEADER() (sizeof(memHeader))

typedef struct memHeader{
    int size;
    int isFree;
    struct memHeader * next;
    struct memHeader * prev;
} memHeader;

memHeader * firstBlock;
int memSize, blockSize;
int bytesAllocated,bytesFree,blocksAllocated;
void splitBlock(memHeader *block, int size);

void updateMemoryStats(int allocatedChange, int freeChange, int blockChange) {
    bytesAllocated += allocatedChange;
    bytesFree += freeChange;
    blocksAllocated += blockChange;
}

void initializeMemoryMM(void *memoryStart, int memorySize)
{
    firstBlock = (memHeader *)memoryStart;
    firstBlock->size = memorySize - (int)SIZE_MEM_HEADER();
    firstBlock->isFree = 1;
    firstBlock->next = NULL;
    firstBlock->prev = NULL;
    updateMemoryStats((int)SIZE_MEM_HEADER(),firstBlock->size,0);
}

void *mallocMM(int size){
    if(size <= 0){
        return NULL;
    }
    memHeader * curr = firstBlock;
    while(curr != NULL && !(curr->isFree && curr->size >= size)){
        curr = curr->next;
    }
    if(curr == NULL){
        return NULL;
    }
    if (curr->size == size)
    {
        updateMemoryStats(size, -size, 1);
        curr->isFree = 0;
        return ((void *)(((void *)curr) + (int)SIZE_MEM_HEADER()));
    }
    if (curr->size > size)
    {

        splitBlock(curr, size);
        updateMemoryStats(size, -size, 1);
        return ((void *)(((void *)curr) + (int)SIZE_MEM_HEADER()));
    }
    return NULL;
}

void freeMM(void *memorySegment)
{
    if (memorySegment != NULL)
    {
        memHeader *curr = (memHeader *)(memorySegment - (int)SIZE_MEM_HEADER());
        curr->isFree = 1;
        updateMemoryStats(-(curr->size), curr->size, -1);
        if (curr->prev != NULL && curr->prev->isFree)
        {
            curr->prev->size += (curr->size + (int)SIZE_MEM_HEADER());
            curr->prev->next = curr->next;
            if(curr->next != NULL){
                curr->next->prev = curr->prev;
            }
            curr = curr->prev;
            updateMemoryStats(-(int)SIZE_MEM_HEADER(),(int)SIZE_MEM_HEADER(),0);

        }
        if (curr->next != NULL && curr->next->isFree)
        {
            updateMemoryStats(-(int)SIZE_MEM_HEADER(),(int)SIZE_MEM_HEADER(),0);
            curr->size += (curr->next->size + (int)SIZE_MEM_HEADER());
            curr->next = curr->next->next;
            if(curr->next != NULL){
                curr->next->prev = curr;
            }
            
        }
    }

}

void splitBlock(memHeader *block, int size)
{
    memHeader *newBlock = NULL;
    if(bytesFree >= (size + SIZE_MEM_HEADER()))
    {
        newBlock = (memHeader *)((void *)block + size + SIZE_MEM_HEADER());
        newBlock->size = block->size - size - (int)SIZE_MEM_HEADER();
        newBlock->isFree = 1;
        newBlock->next = block->next;
        newBlock->prev = block;
        updateMemoryStats((int)SIZE_MEM_HEADER(), -(int)SIZE_MEM_HEADER(), 0);
    }
    block->size = size;
    block->isFree = 0;
    block->next = newBlock;
}

void getMemoryStatus(int* status){
    status[0] = blocksAllocated;
    status[1] = bytesAllocated;
    status[2] = bytesFree;
}
