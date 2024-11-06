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
int testMM(int maxMem, int foreground);
int testPrio(int foreground);
int testProc(int processCount, int foreground);
int testNoSync(int n, int foreground);
int testSync(int n, int foreground);
int testPipe(void);
#endif
