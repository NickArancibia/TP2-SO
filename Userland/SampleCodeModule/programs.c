// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/programs.h"
#include "./include/syscalls.h"
#include "./include/stdio.h"
#include "./include/syscalls.h"
#include "./include/test_util.h"
#include "./include/lib.h"
#include "./include/string.h"

#define MAX_BUFFER 2048

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

void cat()
{
    char c = '\0';
    int idx = 0;
    char buffer[MAX_BUFFER + 1] = {'\0'};
    while ((c = getchar()) != EOF)
    {
        putchar(c);
        if (idx < MAX_BUFFER)
        {
            buffer[idx++] = c;
        }
    }
    print(buffer);
    print("\n");
}

void filter()
{
    char buffer[MAX_BUFFER + 1] = {'\0'};
    char letter[2] = {'\0'};
    int idx = 0;
    while ((letter[0] = getchar()) != EOF)
    {
        putchar(letter[0]);
        if (strcmp(letter, "a") != 0 && strcmp(letter, "e") != 0 && strcmp(letter, "i") != 0 && strcmp(letter, "o") != 0 && strcmp(letter, "u") != 0)
        {
            if (idx < MAX_BUFFER)
            {
                buffer[idx++] = letter[0];
            }
        }
    }
    print(buffer);
    print("\n");
}

void wc()
{
    int count = 0;
    char c;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n')
        {
            count++;
        }
    }
    printf("Total newlines: %d\n", count);
}