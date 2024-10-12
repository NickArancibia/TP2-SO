#ifndef __SYSCALLS_H_
#define __SYSCALLS_H_

#include <stdint.h>
typedef long PID;
#include "./processStructure.h"

int sysWriteScreen(uint64_t fd, unsigned char *buffer, uint64_t len, uint64_t hexColor);
int sysReadScreen(uint64_t fd, unsigned char *buffer, uint64_t len);
void sysSleep(uint64_t secs, uint64_t ms);
int sysClearScreen();
int sysClearKbEntry();

int sysCtrlPressed();

uint8_t sysUpArrowValue();
uint8_t sysLeftArrowValue();
uint8_t sysDownArrowValue();
uint8_t sysRightArrowValue();
int sysPrintRectangle(int x, int y, int base, int height, uint32_t hexColor);
int sysPrintSquare(int x, int y, int side, uint32_t hexColor);
int sysSetCursorPosition(uint64_t x, uint64_t y);
int sysBeepSound(uint64_t ms, uint64_t freq);

int sysGetSecs(void);
int sysGetMins(void);
int sysGetHour(void);

int sysGetDay(void);
int sysGetMonth(void);
int sysGetYear(void);

int sysZoomIn(void);
int sysZoomOut(void);
int sysGetZoomLevel(void);
int sysSetZoomLevel(int zoomLevel);

int sysPrintRegs(void);
int sysPrintCursor();
int sysHideCursor();
int sysShowCursor();
void *sysMalloc(int size);
int sysFree(void *ptrBlock);
int sysCreateProcess(creationParameters *params);
int sysGetPID();
int sysGetParentPID();

Process *sysGetPS();
void sysFreePS(Process *ps);
void sysYield();
int sysSuspendProcess();
int sysResumeProcess();
int sysKill(PID pid);
void sysExit();
void sysWait(PID pid, int *wstatus);
int sysNice(PID pid, Priority priority);
void sysGetMemStatus(int* memStatus);
#endif