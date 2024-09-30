#ifndef _TESTS_H_
#define _TESTS_H_

#include <stdint.h>

void testMM(void);
void helpCommands(void);
void quit(void);
void commandNotFound(char *commandNotFound);

uint64_t test_mm(uint64_t argc, char *argv[]);
#endif