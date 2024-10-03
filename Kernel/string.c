#include <string.h>

int strlen(const char *str)
{
    int size;
    for (size = 0; str[size] != '\0'; size++)
        ;
    return size;
}