// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
void ps();
void getMemoryStatus();
int testMM(int *fds, int isForeground, char *args[]);     // args = {maxMem}
int testPrio(int *fds, int isForeground, char *args[]);   // args = null
int testProc(int *fds, int isForeground, char *args[]);   // args = {processCount}
int testNoSync(int *fds, int isForeground, char *args[]); // args = {n}
int testSync(int *fds, int isForeground, char *args[]);   // args = {n}
int testPipe(int *fds, int isForeground, char *args[]);   // args = {n}
void parseConsolePrompt(char *consolePrompt);
void suspend(char *argv[]);
void resume(char *argv[]);
void nice(char *argv[]);
void killProcess(char *argv[]);
int wcProgram(int *fds, int isForeground, char *args[]);
int catProgram(int *fds, int isForeground, char *args[]);
int filterProgram(int *fds, int isForeground, char *args[]);
int loopProgram(int *fds, int isForeground, char *args[]);
int philo(int *fds, int isForeground, char *args[]);

#endif
