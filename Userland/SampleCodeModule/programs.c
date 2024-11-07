// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/programs.h"
#include "./include/syscalls.h"
#include "./include/stdio.h"
#include "./include/syscalls.h"
#include "./include/test_util.h"

void programDispatcher(creationParameters *params)
{
    int pid = sysCreateProcess(params);
    if (params->foreground)
    {
        sysWait(pid, NULL);
    }
}

void loop(int argc, char **argv)
{
    if (argc != 1 || argv[0] == NULL)
    {
        printf("Usage: loop <seconds to sleep>\n");
        return;
    }
    int n = satoi(argv[0]);
    if (n <= 0)
    {
        printf("Invalid argument\n");
        return;
    }
    int mypid = sysGetPID();
    while (1)
    {
        printf("%d ", mypid);
        sysSleep(n, 0);
    }
}

/*void addPhylo(int ammountOfPhylo, PID *pids, int *phylos, char *semNames[]){

    phylos[0] = NOTEATING;
    phylos[ammountOfPhylo - 1] = NOTEATING;
    sysSuspendProcess(pids[0]);
    sysKill(pids[ammountOfPhylo - 1]);   //lo mato ya que debo cambiar el semaforo

    ammountOfPhylo++;
    //hago un realloc de pids y agrego uno
    //hago un realloc de phylos y agrego uno, le clavo NOEATING
    //hago un realloc de semNames y agrego uno, le clavo fork_"$(ammountOfPhylo)"

    for (int i = ammountOfPhylo - 2; i < ammountOfPhylo; i++)
    {
        char index[1];
        intToString(i,index,1);
        char *argv[] = {semNames[i], semNames[(i+1) % ammountOfPhylo], index, 0};

        creationParameters params;
        params.name = "eachPhylo";
        params.argc = 3;
        params.argv = argv;
        params.priority = 1;
        params.entryPoint = (entryPoint)eachPhylo;
        params.foreground = 1;

        phylos[i] = NOTEATING;
        pids[i] = createProcess(&params);
    }
}*/

int terminatePhylosophers = 0; //global variable that indicates phylosophers must finish
int *phylos;
//int sem_ids[10];

void eachPhylo(int argc, char **argv)
{
    char *fork1 = argv[0];
    char *fork2 = argv[1];
    int index;
    stringToInt(argv[2],&index);

    int fork1_id = sysSemOpen(fork1,1);
    int fork2_id = sysSemOpen(fork2,1);

    while(!terminatePhylosophers){

        sysSemWait(fork1_id);
        sysSemWait(fork2_id);
        phylos[index] = EATING;
        sysSleep(0,10);  //the phylosopher is eating
        sysSemPost(fork1_id);
        sysSemPost(fork2_id);
        phylos[index] = NOTEATING;
        sysSleep(0,10); //the phylosopher is thinking
    }

    sysSemClose(fork1_id);
    sysSemClose(fork2_id);

}

void phylo(int argc, char **argv){

    int ammountOfPhylo = satoi(argv[0]);
    if (ammountOfPhylo <= 0)
    {
        printf("Invalid argument\n");
        return;
    }

    terminatePhylosophers = 0;
    PID *pids = sysMalloc(sizeof(PID) * ammountOfPhylo);
    phylos = sysMalloc(sizeof(int) * ammountOfPhylo);

    char *semNames[5] = {"fork_1","fork_2","fork_3","fork_4","fork_5"};


    for (int i = 0; i < ammountOfPhylo; i++)
    {
        char index[1];
        intToString(i,index,1);
        char *argv[] = {semNames[i], semNames[(i+1) % ammountOfPhylo], index, 0};

        creationParameters params;
        params.name = "eachPhylo";
        params.argc = 3;
        params.argv = argv;
        params.priority = 1;
        params.entryPoint = (entryPoint)eachPhylo;
        params.foreground = 1;
        params.fds[0] = STDIN;
        params.fds[1] = STDOUT;

        phylos[i] = NOTEATING;
        pids[i] = sysCreateProcess(&params);
    }

    sysSleep(0,10); //wait until the proccesses are executed for the first time

    for (int i = 0; i < 10; i++)
    {
        printf(" ");
        for (int j = 0; j < ammountOfPhylo ; j++)
        {
            if(phylos[j] == EATING)
            {
                printf("E ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
        sysSleep(0,30);
    }

    terminatePhylosophers = 1;
    sysSleep(0,50); //hold until phylosophers free resources
    sysFree(pids);
    sysFree(phylos);

    for (int i = 0; i < ammountOfPhylo; i++)
    {
        sysKill(pids[i]);
    }
}
