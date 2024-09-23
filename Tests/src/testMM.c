
#include "../include/testMM.h"
#include "../include/memoryManager.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../include/testUtils.h"
#define MAX_BLOCKS 10

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;


uint64_t test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;
   if (argc != 1)
     return -1;

  printf("You are testing memory manager\n");
  printf("If an error takes place, the proper message will appear\nOtherwise, you will see 'OK!' being printed constantly.\n");
  int iteration = 0;
  if ((max_memory = satoi("25000")) <= 0)
    return -1;

  while (1) {

    rq = 0;
    total = 0;
    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = mallocMM(mm_rqs[rq].size);
      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);
    }

    // Check
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("test_mm ERROR\n");
          return -1;
        }
    }

    // Free
    for (i = 0; i < rq; i++){
      if (mm_rqs[i].address)
        freeMM(mm_rqs[i].address);
    }

    if(iteration){
      printf("\b\b\b");
    }
    printf("OK!");
    iteration =1;
  }
}