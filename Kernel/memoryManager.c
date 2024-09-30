// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <memoryManager.h>
#include "./include/defs.h"

#define BLOCK_COUNT 250

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
        return ((void *)(((void *)curr) + sizeof(memHeader)));
    }
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