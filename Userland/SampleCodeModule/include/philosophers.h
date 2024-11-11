#ifndef __PHILOSOPHERS_H
#define __PHILOSOPHERS_H

#define EATING 1
#define NOTEATING 0

#define MAXNAMELEN 16

#define MAX_PHILOSOPHERS 16
#define MIN_PHILOSOPHERS 2

void getKey();
void printPhiloStates();
char *getPhiloState(int index);
void philosopher(int argc, char *argv[]);

void quitPhilosophers();
void addPhilosopher();
void removePhilosopher();

void philoThink(int index);
void philoEat(int index);

#endif