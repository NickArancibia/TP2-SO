// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/programs.h"
#include "./include/syscalls.h"
#include "./include/stdio.h"
#include "./include/syscalls.h"
#include "./include/test_util.h"
#include "./include/lib.h"

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

char **getSemNames(int ammountOfPhylo)
{
    char ** ans = sysMalloc(sizeof(char *) * ammountOfPhylo);
    for (int i = 0; i < ammountOfPhylo; i++)
    {
        
    }
    
}

int phyloStateHasChanged = 0;
int terminatePhylosophers = 0; //global variable that indicates phylosophers must finish
int *phylos;
int sem_close[10];

void addPhylo(int *ammountOfPhylo, PID *pids, int *phylos, char *semNames[])
{

    phylos[0] = NOTEATING;
    phylos[*ammountOfPhylo - 1] = NOTEATING;
    sysSuspendProcess(pids[0]);
    sem_close[*ammountOfPhylo - 1] = 1;
    //sysSleep(0,50);
    sysKill(pids[*ammountOfPhylo - 1]);   //lo mato ya que debo cambiar el semaforo

    (*ammountOfPhylo)++;
    //hago un realloc de pids y agrego uno
    //hago un realloc de phylos y agrego uno, le clavo NOEATING
    //hago un realloc de semNames y agrego uno, le clavo fork_"$(ammountOfPhylo)"

    for (int i = (*ammountOfPhylo) - 2; i < *ammountOfPhylo; i++)
    {
        char index[2];
        intToString(i,index,1);
        char *argv[] = {semNames[i], semNames[(i+1) % *ammountOfPhylo], index, "0", 0};

        if(i == ammountOfPhylo - 1){
            argv[3] = 1;
        }

        creationParameters params;
        params.name = "eachPhylo";
        params.argc = 4;
        params.argv = argv;
        params.priority = 1;
        params.entryPoint = (entryPoint)eachPhylo;
        params.foreground = 1;
        params.fds[0] = STDIN;
        params.fds[1] = STDOUT;
        sem_close[i] = 0;

        phylos[i] = NOTEATING;
        pids[i] = sysCreateProcess(&params);
        sysSleep(0,30);
    }
    sysResumeProcess(pids[0]);
}

void removePhylo(int *ammountOfPhylo, PID *pids, int *phylos, char *semNames[])
{

    phylos[0] = NOTEATING;
    phylos[*ammountOfPhylo - 1] = NOTEATING;
    sysSuspendProcess(pids[0]);
    sem_close[*ammountOfPhylo - 1] = 1;
    sem_close[*ammountOfPhylo - 2] = 1;
    //sysSleep(0,50);                          //para que de el tiempo a cerrar los sem
    sysKill(pids[*ammountOfPhylo - 1]);      //lo mato ya que debo cambiar el semaforo
    sysKill(pids[*ammountOfPhylo -2]);

    (*ammountOfPhylo)--;

    //hago un realloc de pids y agrego uno
    //hago un realloc de phylos y agrego uno, le clavo NOEATING
    //hago un realloc de semNames y agrego uno, le clavo fork_"$(ammountOfPhylo)"

    char index[2];
    int nextIndex = (*ammountOfPhylo) - 1;
    intToString(nextIndex,index,1);
    char *argv[] = {semNames[nextIndex], semNames[(nextIndex+1) % *ammountOfPhylo], index, "1", 0};

    creationParameters params;
    params.name = "eachPhylo";
    params.argc = 4;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)eachPhylo;
    params.foreground = 1;
    params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    sem_close[nextIndex] = 0;

    phylos[nextIndex] = NOTEATING;
    pids[nextIndex] = sysCreateProcess(&params);
    sysResumeProcess(pids[0]);

}

void eachPhylo(int argc, char **argv)
{
    char *fork1 = argv[0];
    char *fork2 = argv[1];
    int index;
    int isLastOrFirst;
    stringToInt(argv[2],&index);
    stringToInt(argv[2],&isLastOrFirst);

    int fork1_id = sysSemOpen(fork1,1);
    int fork2_id = sysSemOpen(fork2,1);

    int mutex_id = -1;
    if(isLastOrFirst){
        mutex_id = sysSemOpen("mutex",1);
    }

    while(!terminatePhylosophers && !sem_close[index]){

        if(mutex_id != -1){
            sysSemWait(mutex_id);
        }

        sysSemWait(fork1_id);
        sysSemWait(fork2_id);
        phylos[index] = EATING;
        phyloStateHasChanged = 1;
        //printf("Arranca a comer el phylo %d\n",index+1);
        sysSleep(0,10);  //the phylosopher is eating
        sysSemPost(fork1_id);
        sysSemPost(fork2_id);

        if(mutex_id != -1){
            sysSemPost(mutex_id);
        }
        phylos[index] = NOTEATING;
        sysSleep(0,10); //the phylosopher is thinking
    }

    sysSemClose(fork1_id);
    sysSemClose(fork2_id);
}

void phylo(int argc, char **argv){

    int mutexId = sysSemOpen("mutex",1);

    int ammountOfPhylo = satoi(argv[0]);
    if (ammountOfPhylo <= 0)
    {
        printf("Invalid argument\n");
        return;
    }

    terminatePhylosophers = 0;
    PID *pids = sysMalloc(sizeof(PID) * 50); //TODO: no dejar fijo
    phylos = sysMalloc(sizeof(int) * 50); //TODO: no dejar fijo

    char *semNames[10] = {"fork_1","fork_2","fork_3","fork_4","fork_5","fork_6","fork_7","fork_8","fork_9","fork_10"};


    for (int i = 0; i < ammountOfPhylo; i++)
    {
        char index[2];
        intToString(i,index,1);
        char *argv[] = {semNames[i], semNames[(i+1) % ammountOfPhylo], index, "0", 0};

        if(i == 0 || i == ammountOfPhylo -1){
            argv[3] = "1";
        }

        creationParameters params;
        params.name = "eachPhylo";
        params.argc = 4;
        params.argv = argv;
        params.priority = 1;
        params.entryPoint = (entryPoint)eachPhylo;
        params.foreground = 1;
        params.fds[0] = STDIN;
        params.fds[1] = STDOUT;

        phylos[i] = NOTEATING;
        sem_close[i] = 0;
        pids[i] = sysCreateProcess(&params);
    }

    sysSleep(0,10); //wait until the proccesses are executed for the first time

    while(1)
    {
        char charToRead;
        //sysReadScreen(STDIN, &charToRead, 1);
        sysReadScreenAtCurrentPosition(STDIN,&charToRead,1);

        if(charToRead == 'a'){
            sysSemWait(mutexId);
            printf("Agrego phylo %d\n",ammountOfPhylo + 1);
            addPhylo(&ammountOfPhylo, pids, phylos, semNames);
            sysWriteScreen(STDIN," ",1,0xFFF); //para que en la entrada se "resetee" el a
            sysSemPost(mutexId);
        }

        else if(charToRead == 'r' && ammountOfPhylo > 2){
            sysSemWait(mutexId);
            printf("Elimino phylo %d\n",ammountOfPhylo);
            removePhylo(&ammountOfPhylo, pids, phylos, semNames);
            sysWriteScreen(STDIN," ",1,0xFFF); //para que en la entrada se "resetee" el a
            sysSemPost(mutexId);
        }

        if(phyloStateHasChanged){
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
            phyloStateHasChanged = 0;
        }
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
