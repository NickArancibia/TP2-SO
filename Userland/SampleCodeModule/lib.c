// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/lib.h"
#include "include/syscalls.h"
#include <stddef.h>

int incTextSize()
{
    return sysZoomIn();
}

int decTextSize()
{
    return sysZoomOut();
}

int intLength(int num, int minLen)
{
    int length = 0;
    while (num != 0)
    {
        length++;
        num /= 10;
    }
    return length < minLen ? minLen : length;
}

void intToString(int64_t num, char *buf, int minLen)
{
    int length = intLength(num, minLen);
    
    // Convert each digit to a char
    for (int i = length - 1; i >= 0; i--)
    {
        buf[i] = (num % 10) + '0';
        num /= 10;
    }
    buf[length] = '\0';
}

void stringToInt(char *str, int *num)
{
    *num = 0;
    while (*str != '\0')
    {
        *num = *num * 10 + (*str - '0');
        str++;
    }
}

int createProcess(creationParameters *params)
{
    return sysCreateProcess(params);
}

char *strtok(char *str, const char *delim)
{
    static char *input = NULL;
    if (str != NULL)
    {
        input = str;
    }
    if (input == NULL || *input == '\0')
    {
        return NULL;
    }

    char *start = input;
    while (*start && *start == *delim)
    {
        start++;
    }

    if (*start == '\0')
    {
        input = NULL;
        return NULL;
    }

    char *end = start;
    while (*end && *end != *delim)
    {
        end++;
    }

    if (*end != '\0')
    {
        *end = '\0';
        input = end + 1;
    }
    else
    {
        input = end;
    }

    return start;
}