#ifndef __SHELL_H_
#define __SHELL_H_

/**
 * Initializes Shell
 */

#define SELECTED_MODE 0

#define IDLE_MODE 1
#define HELP_MODE 2
#define DIVBYZERO_MODE 3
#define INVALIDOPCODE_MODE 4
#define ZOOMIN_MODE 5
#define ZOOMOUT_MODE 6
#define TIME_MODE 7
#define DATE_MODE 8
#define ELIMINATOR_MODE 9
#define CLEAR_MODE 10
#define REGISTERS_MODE 11
#define EASTEREGG_MODE 12
#define TESTING_AREA 13
#define YIELD 14

int init();

#endif