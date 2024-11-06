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
int testMM(int foreground);
int testPrio(int foreground);
int testProc(int foreground);
int testNoSync(int foreground);
int testSync(int foreground);

#endif
