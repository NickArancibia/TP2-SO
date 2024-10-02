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



typedef struct memHeader
{
    int size;
    int isFree;
    struct memHeader *next;
    struct memHeader *prev;
} memHeader;

memHeader *firstBlock;
int memSize, blockSize;

void splitBlock(memHeader *block, int size);

void initializeMemoryMM(void *memoryStart, int memorySize)
{
    firstBlock = (memHeader *)memoryStart;
    firstBlock->size = memorySize - sizeof(memHeader);
    firstBlock->isFree = 1;
    firstBlock->next = NULL;
    firstBlock->prev = NULL;
}

void *mallocMM(int size)
{
    if (size <= 0)
    {
        return NULL;
    }
    memHeader *curr = firstBlock;

    size += ALIGN(sizeof(memHeader)); // Aligns the block header
    size = ALIGN(size); // Aligs the total size
    while (curr != NULL && !(curr->isFree && curr->size >= size))
    {
        curr = curr->next;
    }
    if (curr == NULL)
    {
        return NULL;
    }
    if (curr->size == size)
    {
        curr->isFree = 0;
        return ((void *)(((void *)curr) + sizeof(memHeader))); // In order to here be sure the pointer
    }                                                          // returned is aligned too
    if (curr->size > size)
    {
        splitBlock(curr, size);
        return ((void *)(((void *)curr) + sizeof(memHeader)));
    }
    return NULL;
}

void freeMM(void *memorySegment)
{
    if (memorySegment != NULL)
    {
        memHeader *curr = (memHeader *)(memorySegment - sizeof(memHeader));
        curr->isFree = 1;
        if (curr->prev != NULL && curr->prev->isFree)
        {
            curr->prev->size += (curr->size + sizeof(memHeader));
            curr->prev->next = curr->next;
            if (curr->next != NULL)
            {
                curr->next->prev = curr->prev;
            }
            curr = curr->prev;
        }
        if (curr->next != NULL && curr->next->isFree)
        {
            curr->size += (curr->next->size + sizeof(memHeader));
            curr->next = curr->next->next;
            if (curr->next != NULL)
            {
                curr->next->prev = curr;
            }
        }
    }
}

void splitBlock(memHeader *block, int size)
{
    memHeader *newBlock = (memHeader *)((void *)block + size + sizeof(memHeader));
    newBlock->size = block->size - size - sizeof(memHeader);
    newBlock->isFree = 1;
    newBlock->next = block->next;
    newBlock->prev = block;
    block->size = size;
    block->isFree = 0;
    block->next = newBlock;
}