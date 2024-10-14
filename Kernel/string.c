// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <string.h>

int strlen(const char *str)
{
    int size;
    for (size = 0; str[size] != '\0'; size++)
        ;
    return size;
}