#ifndef __SYSCALLHANDLE_H_
#define __SYSCALLHANDLE_H_

#include <stdint.h>
#include <process.h>
#include <scheduler.h>
#include "semaphore.h"

int setCursor(uint64_t x, uint64_t y);
int hideCursor();
int showCursor();
int printCursor();

int upArrowValue();
int leftArrowValue();
int downArrowValue();
int rightArrowValue();

int printRect(int x, int y, int base, int height, uint32_t hexColor);
int printSquare(int x, int y, int side, uint32_t hexColor);

int clearScreen();
int msSleep(uint64_t secs, uint64_t ticks); // rdi : ms
int printRegs(void);

int sound(uint64_t ms, uint64_t freq);

int getCurrentSeconds(void);
int getCurrentMinutes(void);
int getCurrentHours(void);

int getCurrentDay(void);
int getCurrentMonth(void);
int getCurrentYear(void);

int incSize();
int decSize();
int getZoomLevel();
int setZoomLevel(int zoomLevel);

int read(uint64_t fd, char *buf, uint64_t count);
int write(uint64_t fd, char *buf, uint64_t count, uint64_t hexColor);

int cleanKbBuffer(void);

int isctrlPressed(void);

void *malloc(int size);

void free(void *memorySegment);

PID processCreate(creationParameters *params);
PID getProcesspid(void);
PID getProcessParentpid(void);
int yield(void);
int suspendProcess(PID pid);
int resumeProcess(PID pid);

Process *getPS(void);
void freePS(Process *processesInfo);
int killProcess(PID pid);
void exit();
void wait(PID pidToWait, int *wstatus);
int nice(PID pid, Priority priority);
void getMemStatus(int *memStatus);
int sem_open(char *sem_id, int initialValue);
void sem_wait(sem_t sem);
void sem_post(sem_t sem);
int sem_close(sem_t sem);
int getFDs(int fds[2]);
int changeFDs(int fds[2]);
int pipes(int fds[2]);
int readAtCurrentPosition(uint64_t fd, char *buf, uint64_t count);
int closeFileDescriptor(uint64_t fd);
#endif