#ifndef __SHELL_H_
#define __SHELL_H_

/**
 * Initializes Shell
 */

#define SELECTED_MODE 0

#define IDLE_MODE 1
#define HELP_MODE 2
#define TIME_MODE 3
#define DATE_MODE 4
#define CLEAR_MODE 5
#define REGISTERS_MODE 6
#define EASTEREGG_MODE 7
#define PS 8
#define YIELD 9
#define KILL 10
#define SUSPEND 11
#define RESUME 12
#define NICE 13
#define MEMORY_STATUS 14
#define TEST_MM 15
#define TEST_PROCS 16
#define TEST_PRIO 17
#define TEST_SYNC 18
#define TEST_NO_SYNC 19

int init();

#endif