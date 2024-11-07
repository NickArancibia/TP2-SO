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