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

int strcmp(const char *str1, const char *str2)
{
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0' && str1[i] == str2[i])
    {
        i++;
    }
    return (unsigned char)str1[i] - (unsigned char)str2[i];
}