#ifndef __PHILOSOPHERS_H
#define __PHILOSOPHERS_H

#define EATING 1
#define NOTEATING 0

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