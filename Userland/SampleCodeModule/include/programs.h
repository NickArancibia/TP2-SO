#ifndef __PROGRAMS_H
#define __PROGRAMS_H

#include "processStructure.h"

void programDispatcher(creationParameters *params);

void loop(int argc, char **argv);

#define NOTEATING 0
#define EATING 1
void phylo(int argc, char **argv);
void eachPhylo(int argc, char **argv);

#endif