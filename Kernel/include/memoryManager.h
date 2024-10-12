#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

void initializeMemoryMM(void *memoryStart, int memorySize);

void *mallocMM(int size);

void freeMM(void *memorySegment);
void getMemoryStatus(int* status);
#endif