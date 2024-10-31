#include <memoryManager.h>
#include <stddef.h>

#define MIN_BLOCK_SIZE 4096
#define MAX_LEVELS 20

typedef struct Block
{
    struct Block *next;
    int level;
} Block;

static Block *freeLists[MAX_LEVELS];
static void *memoryBase;
static int totalMemorySize;

void initializeMemoryMM(void *memoryStart, int memorySize)
{
    memoryBase = memoryStart;
    totalMemorySize = memorySize;

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        freeLists[i] = NULL;
    }

    int initialLevel = 0;
    while ((1 << initialLevel) * MIN_BLOCK_SIZE < memorySize)
    {
        initialLevel++;
    }

    Block *initialBlock = (Block *)memoryStart;
    initialBlock->next = NULL;
    initialBlock->level = initialLevel;
    freeLists[initialLevel] = initialBlock;
}

void *mallocMM(int size)
{
    int level = 0;
    while ((1 << level) * MIN_BLOCK_SIZE < size + sizeof(Block))
    {
        level++;
    }

    if (level >= MAX_LEVELS)
    {
        return NULL;
    }

    for (int i = level; i < MAX_LEVELS; i++)
    {
        if (freeLists[i] != NULL)
        {
            Block *block = freeLists[i];
            freeLists[i] = block->next;

            while (i > level)
            {
                i--;
                Block *buddy = (Block *)((char *)block + (1 << i) * MIN_BLOCK_SIZE);
                buddy->next = freeLists[i];
                buddy->level = i;
                freeLists[i] = buddy;
            }

            block->level = level;
            return (void *)((char *)block + sizeof(Block));
        }
    }

    return NULL;
}

void freeMM(void *memorySegment)
{
    if (memorySegment == NULL)
    {
        return;
    }

    Block *block = (Block *)((char *)memorySegment - sizeof(Block));
    int level = block->level;

    while (level < MAX_LEVELS - 1)
    {
        Block *buddy = (Block *)((char *)memoryBase + (((char *)block - (char *)memoryBase) ^ ((1 << level) * MIN_BLOCK_SIZE)));

        Block **current = &freeLists[level];
        while (*current != NULL && *current != buddy)
        {
            current = &(*current)->next;
        }

        if (*current == buddy)
        {
            *current = buddy->next;
            block = (Block *)((char *)block < (char *)buddy ? (char *)block : (char *)buddy);
            level++;
            block->level = level;
        }
        else
        {
            break;
        }
    }

    block->next = freeLists[level];
    freeLists[level] = block;
}

void getMemoryStatus(int *status)
{
    return;
}