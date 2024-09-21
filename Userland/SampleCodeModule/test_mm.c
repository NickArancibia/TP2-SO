#include "./include/test_util.h"
#include "./include/stdio.h"
#include "./include/stdlib.h"
#include "./include/string.h"
#include "./include/colors.h"
#include "./include/testingArea.h"
#include "./include/syscalls.h"


#define MAX_BLOCKS 250

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

int checkExit(){
  if(getchar() == 'q'){
    printColor("OK!",GREEN);
    print("\n");
      return 1;
  }
  return 0;
}

uint64_t test_mm(uint64_t argc, char *argv[]) {
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;
  // if (argc != 1)
  //   return -1;
  sysHideCursor();
  print("You are testing memory manager\n");
  print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
  print("Press 'q' to finish the test\n\n");
  int iteration = 0;
  if ((max_memory = satoi(argv[0])) <= 0)
     return -1;

  while (1) {

    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      mm_rqs[rq].size = 100;   //GetUniform(max_memory - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size);
      if (checkExit())
        return 0;
      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++){
      if (checkExit())
        return 0;
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);
    }

    // Check
    for (i = 0; i < rq; i++){
      if (checkExit())
        return 0;
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printColor("test_mm ERROR\n",RED);
          return -1;
        }
    }

    // Free
    for (i = 0; i < rq; i++){
      if (checkExit())
        return 0;
      if (mm_rqs[i].address)
        free(mm_rqs[i].address);
    }
    
      if(iteration){
        print("\b\b\b");
      }
    iteration =1;
  }
}
