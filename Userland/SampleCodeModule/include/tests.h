#ifndef __TESTS_H_
#define __TESTS_H_

#include "test_util.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "colors.h"
#include "syscalls.h"
#include "lib.h"
#include "processStructure.h"
#include <stdint.h>

#define MAX_BLOCKS 10
#define WAIT 100000000

#define TOTAL_PROCESSES 3
#define LOWEST 1
#define MEDIUM 3
#define HIGHEST 5
#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 1

int64_t test_processes(uint64_t argc, char *argv[]);
void test_prio();
uint64_t test_mm(uint64_t argc, char *argv[]);
uint64_t test_sync(uint64_t argc, char *argv[]);

#endif