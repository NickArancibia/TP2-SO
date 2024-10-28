#ifndef MODES_H
#define MODES_H

void help(void);
void time(void);
void date(void);
void clear(void);
void registers(void);
void notFound(char *commandNotFound);
void playEasterEgg();
void yield();
void printProcessesInformation();
void getMemoryStatus();
int testMM();
int testPrio();
int testProc();
int testNoSync();
int testSync();

#endif
