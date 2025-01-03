// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/stdio.h"
#include "./include/test_util.h"
#include "./include/syscalls.h"
#include "./include/lib.h"
#include "./include/philosophers.h"

int philoStates[MAX_PHILOSOPHERS];
int philoPIDS[MAX_PHILOSOPHERS];
int forks[MAX_PHILOSOPHERS];
int philoCount;
int hasChanged = 0, finishPhilosophers = 0;

int philoStart(int argc, char *argv[])
{
    if (argc == 0)
    {
        printf("Use: phylo [ammount of philosophers]\n");
        return 1;
    }
    philoCount = satoi(argv[0]);
    if (philoCount < MIN_PHILOSOPHERS || philoCount > MAX_PHILOSOPHERS)
    {
        printf("Invalid ammount of philosophers. Must be between %d and %d\n", MIN_PHILOSOPHERS, MAX_PHILOSOPHERS);
        return 1;
    }

    printf("Welcome to philosophers!\nPress 'q' to end, 'a' to add a philosopher and 'r' to remove one\n");
    sysSleep(1, 0);
    char indexName[MAXNAMELEN] = {0};
    finishPhilosophers = 0;
    for (int i = 0; i < MAX_PHILOSOPHERS; i++)
    {
        forks[i] = sysSemOpen(NULL, 1);
        if (forks[i] == -1)
        {
            printf("Error opening semaphore %d\n", i);
            return 1;
        }
        philoStates[i] = NOTEATING;
        philoPIDS[i] = -1;
    }

    creationParameters params;
    char *argvAux[] = {indexName, NULL};
    // Create philosophers
    for (int i = 0; i < philoCount; i++)
    {
        intToString(i, indexName, 0);
        params.argc = 1;
        params.argv = argvAux;
        params.entryPoint = (entryPoint)philosopher;
        params.foreground = 0;
        params.name = "philosopher";
        params.priority = 1;
        params.fds[0] = STDIN;
        params.fds[1] = STDOUT;
        philoPIDS[i] = sysCreateProcess(&params);
    }

    printPhiloStates();
    return 0;
}

void printPhiloStates()
{
    while (1)
    {
        if (hasChanged)
        {
            hasChanged = 0;
            for (int i = 0; i < philoCount; i++)
            {
                printf(getPhiloState(i));
            }
            printf("\n");
        }
        getKey();
    }
}

void getKey()
{
    unsigned char charToRead = '\0';
    sysReadScreenAtCurrentPosition(STDIN, &charToRead, 1);

    if (charToRead == 'q')
    {
        quitPhilosophers();
    }
    else if (charToRead == 'a')
    {
        addPhilosopher();
    }
    else if (charToRead == 'r')
    {
        removePhilosopher();
    }
}

char *getPhiloState(int index)
{
    switch (philoStates[index])
    {
    case EATING:
        return " E ";
    default:
        return " . ";
    }
}

void philosopher(int argc, char *argv[])
{
    if (argc == 0)
    {
        printf("Philosopher needs index!\n");
        return;
    }

    int index = satoi(argv[0]);
    if (index < 0)
    {
        printf("Invalid index\n");
        return;
    }

    while (1)
    {
        philoThink(index);
        if (index >= philoCount || finishPhilosophers)
            break;
        philoEat(index);
        if (index >= philoCount || finishPhilosophers)
            break;
    }
}

void addPhilosopher()
{
    if (philoCount == MAX_PHILOSOPHERS)
    {
        printf("Max philosophers reached\n");
        return;
    }
    philoCount++;
    printf("A new philosopher has just arrived! \n");
    char indexName[MAXNAMELEN] = {0};
    char *argvAux[] = {indexName, NULL};
    intToString(philoCount - 1, indexName, 0);

    creationParameters params;
    params.argc = 1;
    params.argv = argvAux;
    params.entryPoint = (entryPoint)philosopher;
    params.foreground = 0;
    params.name = "philosopher";
    params.priority = 1;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    sysCreateProcess(&params);
}

void removePhilosopher()
{
    if (philoCount == MIN_PHILOSOPHERS)
    {
        printf("Min philosophers reached\n");
        return;
    }
    philoCount--;

    printf("A philosopher leaves the table... \n");
}

void quitPhilosophers()
{
    finishPhilosophers = 1;
    for (int i = 0; i < philoCount; i++)
    {
        sysWait(philoPIDS[i], NULL);
    }
    for (int i = 0; i < MAX_PHILOSOPHERS; i++)
    {
        sysSemClose(forks[i]);
    }
    printf("Everyone leaves hoping to see each other again soon...\n");
    sysExit();
}

void philoThink(int index)
{
    int thinkTime = GetUniform(index) % 20;
    sysSleep(0, thinkTime);
}

void philoEat(int index)
{
    int currentCount = philoCount;
    int leftFork = forks[index];
    int rightFork = forks[(index + 1) % currentCount];

    if (index % 2 == 0)
    {
        sysSemWait(leftFork);
        sysSemWait(rightFork);
    }
    else
    {
        sysSemWait(rightFork);
        sysSemWait(leftFork);
    }
    philoStates[index] = EATING;
    hasChanged = 1;
    sysSleep(0, 10); // Eating

    sysSemPost(rightFork);
    sysSemPost(leftFork);

    philoStates[index] = NOTEATING;
    hasChanged = 1;
}