
#ifndef MEMORY_H
#define MEMORY_H

typedef struct memHeader{
    int size;
    int isFree;
    struct memHeader * next;
    struct memHeader * prev;
} memHeader;

#define SIZE_BLOCK_CONTROL sizeof(memHeader)



void initializeMemoryMM(void * memoryStart, int memorySize);

void * mallocMM(int size);

int freeMM(void * memorySegment);



#endif //MEMORY_H