#include <memoryManager.h>

#include <stddef.h>
#define MIN_BLOCK_SIZE 8
#define MAX_LEVELS 20

typedef struct Block
{
    struct Block *next;
    int level;
} Block;

int bytesAllocated, bytesFree, blocksAllocated, bytesTotal;

static Block *buddyLists[MAX_LEVELS];
static void *memoryBase;

void updateMemoryStats(int allocatedChange, int freeChange, int blockChange)
{
    bytesAllocated += allocatedChange;
    bytesFree += freeChange;
    blocksAllocated += blockChange;
}

void initializeMemoryMM(void *memoryStart, int memorySize)
{
    bytesFree = memorySize;
    memoryBase = memoryStart;

    for (int i = 0; i < MAX_LEVELS; i++)
    {
        buddyLists[i] = NULL;
    }

    int initialLevel = 0;
    while ((1 << initialLevel) * MIN_BLOCK_SIZE < memorySize)
    {
        initialLevel++;
    }

    Block *initialBlock = (Block *)memoryStart;
    initialBlock->next = NULL;
    initialBlock->level = initialLevel;
    buddyLists[initialLevel] = initialBlock;
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
        if (buddyLists[i] != NULL)
        {
            Block *block = buddyLists[i];
            buddyLists[i] = block->next;

            while (i > level) // We divide the block if it is bigger than the requested size
            {
                i--;
                Block *buddy = (Block *)((void *)block + (1 << i) * MIN_BLOCK_SIZE); // Divide the block
                buddy->next = buddyLists[i];
                buddy->level = i;
                buddyLists[i] = buddy;
            }

            block->level = level;
            updateMemoryStats((1 << level) * MIN_BLOCK_SIZE, -1 * (1 << level) * MIN_BLOCK_SIZE, 1);
            return (void *)((void *)block + sizeof(Block));
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

    Block *block = (Block *)((void *)memorySegment - sizeof(Block));
    int level = block->level;
    int freedSize = (1 << level) * MIN_BLOCK_SIZE;

    while (level < MAX_LEVELS - 1)
    {
        Block *buddy = (Block *)((void *)memoryBase + (((void *)block - (void *)memoryBase) ^ ((1 << level) * MIN_BLOCK_SIZE)));

        Block **current = &buddyLists[level];
        while (*current != NULL && *current != buddy)
        {
            current = &(*current)->next;
        }

        if (*current == buddy)
        {
            *current = buddy->next;
            block = (Block *)((void *)block < (void *)buddy ? (void *)block : (void *)buddy);
            level++;
            block->level = level;
        }
        else
        {
            break;
        }
    }

    updateMemoryStats(-1 * freedSize, freedSize, -1);
    block->next = buddyLists[level];
    buddyLists[level] = block;
}

void getMemoryStatus(int *status)
{
    status[0] = blocksAllocated;
    status[1] = bytesAllocated;
    status[2] = bytesFree;
    status[3] = bytesFree + bytesAllocated;
}
