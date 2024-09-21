#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

void initializeMemoryMM(void * memoryStart, int memorySize);

void * mallocMM(int size);

int freeMM(void * memorySegment);

#endif