#include <memoryManager.h>

#define BLOCK_COUNT 250

void * start;
int size;

void *free_ptrs[BLOCK_COUNT];
int current;

void initializeMemoryMM(void * memoryStart, int memorySize){
        start = memoryStart;
        
        for( int i = 0 ; i < BLOCK_COUNT ; i++){
                free_ptrs[i] = memoryStart + (memorySize / BLOCK_COUNT) * i;
        }
        current = 0;
}

void *mallocMM(){
    
    if(current < BLOCK_COUNT){ //si no me pase del arreglo
            /*current++;
            return free_ptrs[current -1];*/
            return free_ptrs[current++];
    }
    return;
    
}

int freeMM(void * memorySegment){
    free_ptrs[--current] = memorySegment;        
    
}


/*int main(){
    //char mem[BLOCK_COUNT * BLOCK_SIZE];
    
    //initializeMemoryMM(mem, BLOCK_COUNT * BLOCK_SIZE);
    
    
    //call test_mm
} */