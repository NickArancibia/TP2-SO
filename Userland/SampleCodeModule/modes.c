// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/modes.h"
#include "include/shell.h"
#include "include/stdio.h"
#include "include/string.h"
#include "include/syscalls.h"
#include "include/lib.h"
#include "include/dateTime.h"
#include "include/colors.h"
#include "include/utils.h"
#include "include/processStructure.h"
#include "include/test_util.h"
#include "include/tests.h"
#include "include/string.h"
#define MAX_BUFF_LEN 30
#define MAX_COMMANDS 2
#define MAX_ARGS 5


char * commands[MAX_COMMANDS] = {NULL};
char * args[MAX_COMMANDS][MAX_ARGS] = {NULL};
int fds[MAX_COMMANDS][2];
char *dateTimeAux;
int zoomAux, regAux;
int memoryStatus[4];
int commandsToExecute = 0;
int pipeFlag = 0;

static char *helpText[] = {"Command information is displayed below:\n\n",
                           "HELP                ->      Shows a description on each available command.\n",
                           "TIME                ->      Shows current time in HH:MM:SS format.\n",
                           "DATE                ->      Shows current date in DD/MM/YY format.\n",
                           "CLEAR               ->      Clears the screen\n",
                           "REGISTERS           ->      Prints registers values. To do this, first you need to save\n",
                           "                            your registers by pressing ALT.\n",
                           "YIELD               ->      Relinquish voluntarily the CPU\n",
                           "PS                  ->      Shows information about all processes\n",
                           "KILL [pid]          ->      Kills the process with the given pid\n",
                           "SUSPEND [pid]       ->      Suspend a process with the given pid.\n",
                           "RESUME [pid]        ->      Resume a process with the given pid.\n",
                           "NICE [pid] [prio]   ->      Change the priority of the process with the given pid to prio.\n",
                           "MEMSTATUS           ->      Prints how many blocks and bytes were allocated \n",
                           "                            and how many bytes are still free.\n",
                           "TESTMM              ->      Test memory manager.\n",
                           "TESTPROC            ->      Test processes creation.\n",
                           "TESTPRIO            ->      Test processes priority.\n",
                           "TESTSYNC            ->      Test syncronization with semaphores.\n",
                           "TESTNOSYNC          ->      Test syncronization without semaphores.\n",
                           "TESTPIPE            ->      Test pipes.\n",
                           "end"};

char *states[5] = {"Ready", "Running", "Blocked", "Dead", "Foreground"};



typedef int (*CommandFunc)(int *, int, char **);
typedef struct {
    char *name;
    CommandFunc func;
    int expectedArgs;
    int isBuiltIn;
} Command;


int64_t outputP(uint64_t argc, char *argv[]);
int64_t inputP(uint64_t argc, char *argv[]);

void loopCreate(int* r, int e,char **argv){}
Command commandList[] = {
    {"help", (CommandFunc)help, 0, 1},
    {"clear",(CommandFunc) clear, 0, 1},
    {"time",(CommandFunc) time, 0, 1},
    {"date",(CommandFunc) date, 0, 1},
    {"registers", (CommandFunc)registers, 0, 1},
    {"yield",(CommandFunc) yield, 0, 1},
    {"ps", (CommandFunc)printProcessesInformation, 0, 1},
    {"kill", (CommandFunc)killProcess, 1, 1},
    {"suspend", (CommandFunc)suspend, 1, 1},
    {"resume", (CommandFunc)resume, 1, 1},
    {"nice", (CommandFunc)nice, 2, 1},
    {"memstatus",(CommandFunc) getMemoryStatus, 0, 1},
    {"testmm", testMM, 1, 0},
    {"testproc", testProc, 1, 0},
    {"testprio", testPrio, 0, 0},
    {"testsync", testSync, 1, 0},
    {"testnosync", testNoSync, 1, 0},
    {"testpipe", testPipe, 1, 0},
    {"loop", (CommandFunc)loopCreate, 0, 0},
    {"inputalone",(CommandFunc)inputP,0,1},
    {"outputalone",(CommandFunc)outputP,0,1},
    {NULL, (CommandFunc)notFound, 0, 1} // Sentinel value to mark the end of the array
};


void help(void)
{
    for (int i = 0; strcasecmp(helpText[i], "end") != 0; i++)
    {
        printColor(helpText[i], YELLOW);
    }
}

void clear(void)
{
    sysClearScreen();
}

void time(void)
{
    printTime();
}

void date(void)
{
    printDate();
}

void registers()
{
    regAux = sysPrintRegs();
    if (regAux)
    {
        print("You need to save registers first by pressing ALT\n");
    }
}

void yield()
{
    sysYield();
}

void notFound(char *commandNotFound)
{
    print(commandNotFound);
    print(": command not found.\n");
}

void playEasterEgg()
{
    printColor("I'm", RED);
    printColor(" an", YELLOW);
    printColor(" eas", GREEN);
    printColor("ter ", BLUE);
    printColor("egg", WHITE);
    printColor("!\n", YELLOW);

    sysHideCursor();
    sysPrintCursor();

    sysBeepSound(220, DO);
    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);

    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, DO);
    for (int i = 0; i < 2; i++)
    {
        sysBeepSound(220, SOL_SOST);
        sysBeepSound(220, FA);
    }

    sysBeepSound(220, DO);
    sysBeepSound(220, DO_SOST);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);
    sysBeepSound(220, SOL);
    sysBeepSound(220, MI);

    sysBeepSound(110, FA);
    sysBeepSound(165, FA_SOST);
    sysBeepSound(110, SOL);
    sysBeepSound(165, SOL_SOST);
    sysBeepSound(110, LA);
    sysBeepSound(165, SI);
    sysBeepSound(110, DO_PRIMA);

    sysShowCursor();
    sysPrintCursor();
}

void ps()
{
    Process *processes = sysGetPS();
    for (int i = 0; processes[i].pid != -1; i++)
    {
        printf("PID=%d | Name=",
               (int32_t)processes[i].pid);
        print(processes[i].name);
        printf(" | ParentPID=%d | Priority=%d | Foreground=%d | State=",
               (int32_t)processes[i].parentpid,
               (int32_t)processes[i].priority,
               (int32_t)processes[i].foreground);
        print(states[processes[i].state]);
        printf(" | StackBase=0x");
        print(hexToString((uint64_t)processes[i].stackBase));
        printf(" | StackEnd=0x");
        print(hexToString((uint64_t)processes[i].stackEnd));
        printf("\n");
    }
    sysFreePS(processes);
}

void printProcessesInformation()
{
    PID pid;
    creationParameters params;
     params.fds[0] = STDIN;
    params.fds[1] = STDOUT;
    params.name = "ps";
    params.argc = 0;
    params.argv = NULL;
    params.priority = 1;
    params.entryPoint = (entryPoint)ps;
    params.foreground = 1;
    pid = createProcess(&params);
    sysWait(pid, NULL);
    return;
}

void getMemoryStatus()
{
    sysGetMemStatus(memoryStatus);
    printf("Memoria total= %d B |  ", memoryStatus[3]);
    printf("Memoria ocupada= %d B (%d)|  ", memoryStatus[1], memoryStatus[1] * 100 / memoryStatus[3]);
    printf("Memoria libre= %d B | ", memoryStatus[2]);
    printf("Bloques ocupados= %d\n", memoryStatus[0]);
}

void killProcess(char *argv[]){
    int pid = satoi(argv[0]);
    if (pid == -1)  
    {
        perror("Invalid PID\n");
        return;
    }
    if (pid == sysGetPID())
    {
        perror("You can't kill the shell\n");
        return;
    }
    if (pid == 1)
    {
        perror("You can't kill idle\n");
        return;
    }
    sysKill(pid);
}

void suspend(char *argv[]){
    int pid = satoi(argv[0]);
    if (pid == -1)  
    {
        perror("Invalid PID\n");
        return;
    }
    if (pid == sysGetPID())
    {
        perror("You can't suspend the shell\n");
        return;
    }
    if (pid == 1)
    {
        perror("You can't suspend idle\n");
        return;
    }
    sysSuspendProcess(pid);
}

void resume(char *argv[]){
    int pid = satoi(argv[0]);
    if (pid == -1)  
    {
        perror("Invalid PID\n");
        return;
    }
    sysResumeProcess(pid);
}

void nice(char *argv[]){
    int pid = satoi(argv[0]);
    int prio = satoi(argv[1]);
    if (pid == -1)  
    {
        perror("Invalid PID\n");
        return;
    }
    if (prio < 0 || prio > 5)
    {
        perror("Invalid priority\n");
        return;
    }
    sysNice(pid, prio);
}


int testProc(int *fds, int isForeground,char *args[])
{
    char count[MAX_BUFF_LEN];
    intToString(satoi(args[0]), count, 0);
    sysHideCursor();
    print("You are testing processes\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    print("Press 'q' to finish the test\n\n");
    char *argv[] = {count, 0};
    creationParameters params;

     params.fds[0] = fds[0];
    params.fds[1] = fds[1];
    params.name = "test_processes";
    params.argc = 1;
    params.argv = argv;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_processes;
    params.foreground = isForeground;

    return createProcess(&params);
}

int testPrio(int *fds, int isForeground,char *args[])
{
    sysHideCursor();
    print("You are testing processes priorities\n");
    print("Below you will see information on running processes\n");
    print("Each process will print its PID an amount of times based on its priority\n\n");
    creationParameters params;
    params.name = "test_prio";
    params.argc = 0;
     params.fds[0] = fds[0];
    params.fds[1] = fds[1]; 
    params.argv = NULL;
    params.priority = 1;
    params.entryPoint = (entryPoint)test_prio;
    params.foreground = isForeground;

    return createProcess(&params);
}

int testMM(int *fds, int isForeground,char *args[])
{
    char max[MAX_BUFF_LEN];
    intToString(satoi(args[0]), max, 0);
    sysHideCursor();
    print("You are testing memory manager\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {max, 0};
    creationParameters params;
    params.name = "test_mm";
    params.argc = 1;
    params.argv = argv;
     params.fds[0] = fds[0];
    params.fds[1] = fds[1]; 
    params.priority = 1;
    params.entryPoint = (entryPoint)test_mm;
    params.foreground = isForeground;   
    return createProcess(&params);
}

int testSync(int *fds, int isForeground,char *args[])
{
    char start[MAX_BUFF_LEN];
    intToString(satoi(args[0]), start, 0);
    sysHideCursor();
    print("You are testing syncronization with semaphores\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {start, "1", 0};
    creationParameters params;
    params.name = "test_sync";
    params.argc = 2;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = fds[0];    
    params.fds[1] = fds[1]; 
    params.entryPoint = (entryPoint)test_sync;
    params.foreground = isForeground;
    return createProcess(&params);
}

int testNoSync(int *fds, int isForeground,char *args[]) 
{
    char start[MAX_BUFF_LEN];
    intToString(satoi(args[0]), start, 0);
    sysHideCursor();
    print("You are testing syncronization without semaphores\n");
    print("If an error takes place, the proper message will appear\nOtherwise, nothing will happen\n");
    char *argv[] = {start, "0", 0};
    creationParameters params;
    params.name = "test_sync";
    params.argc = 2;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = fds[0];
    params.fds[1] = fds[1];
    params.entryPoint = (entryPoint)test_sync;
    params.foreground = isForeground;
    return createProcess(&params);
}

int64_t inputP(uint64_t argc, char *argv[]){
    char buffer[100] = {'\0'};
    
    while(scanf(buffer, 100) != EOF){
        if (strcmp(buffer, "exit") == 0)
        {
            return 0;
        }  
    }
    return 0;
}
 
int64_t outputP(uint64_t argc, char *argv[]){
    char buffer[100] = {'\0'};
    while(scanf(buffer, 100) != EOF){
        if (strcmp(buffer, "exit") == 0)
        {
            return 0;
        }  
    }
    return 0;
}

int testPipe(int *fileDescriptors, int isForeground,char *args[])
{
    sysHideCursor();
    print("Testing pipes\n");
    print("Two processes were created, one reads from STDIN, the other writes to STDOUT \n");
    print("this test works like 'input | output'\n");
    print("Type 'exit' and press enter to exit\n");
    char *argv[] = {0};
    int fds[2];
    int pids[2];
    sysPipe(fds);
    creationParameters params;
    params.name = "inputP";
    params.argc =0;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = STDIN;
    params.fds[1] = fds[1];
    params.entryPoint = (entryPoint)inputP;
    params.foreground = isForeground;
    pids[0] = createProcess(&params);
    params.name = "outputP";
    params.argc =0;
    params.argv = argv;
    params.priority = 1;
    params.fds[0] = fds[0];
    params.fds[1] = STDOUT;
    params.entryPoint = (entryPoint)outputP;
    params.foreground = isForeground;
    pids[1] = createProcess(&params);
    if (isForeground)
    {
        sysWait(pids[0], NULL);
        sysWait(pids[1], NULL); 
    }
    return 0;
}

int closeFDsmadeByParser(){
    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        sysCloseFD(fds[i][0]);
        sysCloseFD(fds[i][1]);
    }
    return 0;
}

void executeCommands(int isForeground[MAX_COMMANDS]){
    int execute[MAX_COMMANDS];
    int pids[MAX_COMMANDS] = {-1};
    int testPipeFlag = 0;
    for (int i = 0; i < commandsToExecute; i++) {
        int expectedArgs;
        int isBuiltIn;

        CommandFunc func = NULL;
        int j;
        for ( j = 0; commandList[j].name != NULL; j++) {
            if (strcmp(commands[i], commandList[j].name) == 0) {
                if (strcmp(commandList[j].name, "testpipe") == 0)
                {
                    testPipeFlag = 1;
                }
                func = commandList[j].func;
                expectedArgs = commandList[j].expectedArgs;
                isBuiltIn = commandList[j].isBuiltIn;        
                break;
            }
        }

        if (func == NULL) {
            closeFDsmadeByParser();
            notFound(commands[i]);
            return;
        }

        if (isBuiltIn && !isForeground[i]) {
            closeFDsmadeByParser();
            perror("Error: Cannot run built-in commands in background\n");
            return;
        }
       
        if (args[i][expectedArgs] != NULL) {
            closeFDsmadeByParser();
            perror("Error: Invalid number of arguments for command '");
            perror(commands[i]);
            perror("'\n");
            return;
        }
        execute[i] = j;
    }
    if (pipeFlag && testPipeFlag) 
    {
        closeFDsmadeByParser();
        perror("Error: Cannot pipe testpipe command\n");
        return;
    }

    for (int i = 0; i < commandsToExecute; i++) {
        if (!commandList[execute[i]].isBuiltIn) {
            pids[i] = commandList[execute[i]].func(fds[i],isForeground[i],args[i]);
        }
        else if (commandList[execute[i]].isBuiltIn && !pipeFlag)
        {
            void (*func)(char **) = (void (*)(char **))commandList[execute[i]].func;
            func(args[i]);
            return;
        }
        
        else{
            creationParameters params;
            params.name = commandList[execute[i]].name;
            params.argc = commandList[execute[i]].expectedArgs;
            params.argv = args[i];
            params.priority = 1;
            params.entryPoint = (entryPoint)commandList[execute[i]].func;
            params.foreground = isForeground[i];
            params.fds[0] = fds[i][0];
            params.fds[1] = fds[i][1];
            pids[i] = createProcess(&params);
        }   
    }
    
    for (int i = 0; i < commandsToExecute; i++) {
        if (isForeground[commandsToExecute-1]) {
            sysWait(pids[i], NULL);
        }
    }   
}

void parseConsolePrompt(char *consolePrompt) {
    char *token;
    int pipe[2];
    token = strtok(consolePrompt, " ");
    int argsc = 0, commandsCount = 0;
    pipeFlag = 0;
    memset(commands, 0, sizeof(commands));
    memset(args, 0, sizeof(args));
    for (int i = 0; i < MAX_COMMANDS; i++)
    {
        fds[i][0] = STDIN;
        fds[i][1] = STDOUT;
    }
    commandsToExecute = 0;
    int isForeground[MAX_COMMANDS] = {1,1};
    while (token != NULL) {
        commands[commandsCount] = token;
        token = strtok(NULL, " ");
        argsc = 0;

        while (token != NULL && strcmp(token, "|") != 0 && strcmp(token, "&") != 0) {
            if (argsc >= MAX_ARGS) {
                closeFDsmadeByParser();
                perror("Error: You exceeded the maximum number of arguments\n");
                return;
            }
            args[commandsCount][argsc++] = token;
            token = strtok(NULL, " ");
        }

        args[commandsCount][argsc] = NULL;

        if (token != NULL && strcmp(token, "&") == 0) {
            isForeground[commandsCount] = 0;
            token = strtok(NULL, " ");
            if (token != NULL && strcmp(token, "|") != 0) {
                closeFDsmadeByParser();
                perror("Error: Invalid command, NULL or '|' expected after '&'\n");
                return;
            }
        }

        if (token != NULL && strcmp(token, "|") == 0) {
            if(!isForeground[commandsCount]){
                closeFDsmadeByParser();
                perror("Error: Cannot pipe background commands\n");
                return;
            }
            if (pipeFlag) {
                closeFDsmadeByParser();
                perror("Error: Only one pipe is allowed\n");
                return;
            }
            pipeFlag = 1;
            token = strtok(NULL, " ");
            if (token == NULL || strcmp(token, "|") == 0 || strcmp(token, "&") == 0) {
                perror("Error: another command was expected\n");
                return;
            }
            sysPipe(pipe);
            fds[commandsCount][1] = pipe[1];
            fds[commandsCount + 1][0] = pipe[0];
        }

        commandsCount++;
    } 
    commandsToExecute = commandsCount;
    executeCommands(isForeground);
}