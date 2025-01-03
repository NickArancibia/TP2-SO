// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/stdio.h"
#include "include/syscalls.h"
#include "include/colors.h"
#include "include/string.h"
#include "include/lib.h"
#include <stdarg.h>
#define MAXBUFLEN 100
#define MINLEN 1
#define MAX_INPUTS_STORE 10

/**
 * readSizeFlag is used as a way of identifying whether
 * getChar returned with no value read because of timeout
 * or not
 */
static int readSizeFlag = 0;

static uint64_t hexcol = DEFAULT; // default shell color

static int isPrintable(unsigned char c);
static int isVerticalArrow(unsigned char c);

static char inputs[MAX_INPUTS_STORE][MAXBUFLEN];
static int inputIndex = 0;

unsigned char getchar(void)
{
    int fds[2];
    sysGetFDs(fds);
    unsigned char read = EOF;
    readSizeFlag = sysReadScreen(fds[0], &read, 1);
    return read;
}

unsigned char getcharAtCurrentPosition(void)
{
    int fds[2];
    sysGetFDs(fds);
    unsigned char read = EOF;
    readSizeFlag = sysReadScreenAtCurrentPosition(fds[0], &read, 1);
    return read;
}

unsigned char putchar(unsigned char c)
{
    int fds[2];
    sysGetFDs(fds);
    sysWriteScreen(fds[1], &c, 1, hexcol);
    return c;
}

int printColor(char *str, uint64_t hexColor)
{
    hexcol = hexColor;
    int i;
    for (i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
    hexcol = DEFAULT;
    return i;
}

int print(char *str)
{
    return printColor(str, DEFAULT);
}
int perror(char *str)
{
    int len = strlen(str) + 1;
    sysWriteScreen(STDERR, (unsigned char *)str, len, RED);
    return 0;
}

int printf(char *str, ...)
{
    char buffer[MAXBUFLEN], numStr[MAXBUFLEN];
    int bufferIndex = 0;
    va_list args;
    va_start(args, str);

    for (int i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] == '%' && str[i + 1] == 'd')
        {
            i++;
            intToString((int64_t)(va_arg(args, int)), numStr, MINLEN);
            for (int j = 0; numStr[j] != '\0'; ++j)
            {
                buffer[bufferIndex++] = numStr[j];
            }
        }
        else
        {
            buffer[bufferIndex++] = str[i];
        }
    }
    buffer[bufferIndex] = '\0';
    va_end(args);
    return print(buffer);
}

int scanf(char *buffer, int size)
{
    if (size == 0)
        return 0;
    unsigned char read = 0;
    int readSize = 0, printedSize = 0;
    while (read != '\n')
    {
        read = getchar();
        if (!readSizeFlag)
            break;

        if (read == '\n')
        {
            buffer[readSize] = 0;
            if (readSize > 0)
                strcpy(inputs[inputIndex++ % MAX_INPUTS_STORE], buffer);
            putchar(read); // Newline
        }
        else if (read == '\b' && readSize != 0)
        {
            if (readSize >= printedSize)
                readSize--;
            buffer[readSize] = 0;
            putchar(read); // Backspace
            printedSize--;
        }
        else if (isVerticalArrow(read))
        {
            if (read == sysUpArrowValue())
            {
                if (inputIndex > 0)
                {
                    inputIndex--;
                }
            }
            else
            {
                if (inputIndex < MAX_INPUTS_STORE)
                {
                    inputIndex++;
                }
            }
            strcpy(buffer, inputs[inputIndex % MAX_INPUTS_STORE]);
            int commandLen = strlen(buffer);
            for (int i = 0; i < printedSize; i++)
                putchar('\b');
            printedSize = commandLen;
            readSize = commandLen;
            print(buffer);
        }
        else if (isPrintable(read))
        { // Printable Character
            if (readSize != size)
                buffer[readSize++] = read;
            putchar(read);
            printedSize++;
        }
    }
    return readSize;
}

static int isPrintable(unsigned char c)
{
    return (c >= ' ' && c <= '~');
}

static int isVerticalArrow(unsigned char c)
{
    return c == sysUpArrowValue() || c == sysDownArrowValue();
}