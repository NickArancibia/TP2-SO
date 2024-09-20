#ifndef __STDLIB_H_
#define __STDLIB_H_



// allocates a single block of a fixed size so far and returns a pointer to it
void * malloc();

// deallocate the block pointed by ptrBlock
void free(void * ptrBlock);

#endif