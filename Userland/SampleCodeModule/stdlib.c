#include "include/stdlib.h"
#include "include/syscalls.h"

/**
 * Asks for memory chunks.
 * Returns NULL if size requested is bigger than 100Bytes or
 * there is no empty chunk in memory.
 * Otherwise returns a pointer to the allocated
 * memory address.
 */
void * malloc(int size){
    return sysMalloc(size);
}

/**
 * Deallocate the block pointed by ptrBlock
*/
void free(void * ptrBlock){
    sysFree(ptrBlock);
}