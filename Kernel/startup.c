#include <startup.h>
#include <videoDriver.h>
#include <speaker.h>
#include <time.h>

void drawLetter(int x, int y, char letter, uint32_t color)
{
    switch (letter)
    {
    case 'S':
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x, y + 5, 5, 10, color);
        vdPrintRect(x, y + 15, 20, 5, color);
        vdPrintRect(x + 15, y + 20, 5, 10, color);
        vdPrintRect(x, y + 30, 20, 5, color);
        break;
    case 'O':
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x, y + 5, 5, 25, color);
        vdPrintRect(x + 15, y + 5, 5, 25, color);
        vdPrintRect(x, y + 30, 20, 5, color);
        break;
    case 'M':
        vdPrintRect(x, y, 5, 35, color);
        vdPrintRect(x + 15, y, 5, 35, color);
        vdPrintRect(x + 5, y + 5, 5, 5, color);
        vdPrintRect(x + 10, y + 5, 5, 5, color);
        vdPrintRect(x + 7, y + 10, 6, 6, color);
        break;
    case 'E':
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x, y + 5, 5, 25, color);
        vdPrintRect(x, y + 15, 15, 5, color);
        vdPrintRect(x, y + 30, 20, 5, color);
        break;
    case 'R':
        vdPrintRect(x, y, 5, 35, color);
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x, y + 15, 15, 5, color);
        vdPrintRect(x + 15, y + 5, 5, 10, color);
        vdPrintRect(x + 10, y + 20, 5, 15, color);
        break;
    case 'C':
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x, y + 5, 5, 25, color);
        vdPrintRect(x, y + 30, 20, 5, color);
        break;
    case 'H':
        vdPrintRect(x, y, 5, 35, color);
        vdPrintRect(x + 15, y, 5, 35, color);
        vdPrintRect(x, y + 15, 20, 5, color);
        break;
    case 'A':
        vdPrintRect(x, y, 15, 5, color);
        vdPrintRect(x, y, 5, 35, color);
        vdPrintRect(x + 15, y, 5, 35, color);
        vdPrintRect(x + 5, y + 15, 10, 5, color);
        break;
    case 'N':
        vdPrintRect(x, y, 5, 35, color);
        vdPrintRect(x + 15, y, 5, 35, color);
        vdPrintRect(x + 5, y + 5, 5, 5, color);
        vdPrintRect(x + 10, y + 10, 5, 5, color);
        vdPrintRect(x + 15, y + 15, 5, 5, color);
        break;
    case 'T':
        vdPrintRect(x, y, 20, 5, color);
        vdPrintRect(x + 7, y, 5, 35, color);
        break;
    }
}

void drawText(const char *text, int x, int y, uint32_t color)
{
    for (int i = 0; text[i] != '\0'; i++)
    {
        drawLetter(x + i * 25, y, text[i], color);
    }
}

void drawBootLogo()
{
    int screenWidth = getScreenWidth();
    int screenHeight = getScreenHeight();

    uint32_t lightGray = 0xA0A0A0;
    uint32_t white = 0xFFFFFF;
    uint32_t orange = 0xFFA500;
    uint32_t black = 0x000000;

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    vdPrintRect(centerX - 40, centerY - 60, 80, 120, lightGray);

    vdPrintRect(centerX - 35, centerY - 30, 70, 80, white);

    vdPrintRect(centerX - 40, centerY - 100, 80, 40, lightGray);

    vdPrintRect(centerX - 25, centerY - 85, 15, 15, white);
    vdPrintRect(centerX + 10, centerY - 85, 15, 15, white);

    vdPrintRect(centerX - 20, centerY - 80, 6, 6, black);
    vdPrintRect(centerX + 15, centerY - 80, 6, 6, black);

    vdPrintRect(centerX - 10, centerY - 65, 20, 10, orange);

    vdPrintRect(centerX - 25, centerY + 60, 15, 10, orange);
    vdPrintRect(centerX + 10, centerY + 60, 15, 10, orange);

    vdPrintRect(centerX - 65, centerY - 30, 25, 10, lightGray);
    vdPrintRect(centerX + 40, centerY - 30, 25, 10, lightGray);

    drawText("SO", centerX - 20, centerY - 160, white);

    drawText("MERCHANTS", centerX - 110, centerY + 100, white);
}
void playBootSound()
{
    beep(6, 523);
    sleep(1);
    beep(6, 659);
    sleep(1);
    beep(6, 784);
    sleep(1);
    beep(6, 1047);
    sleep(1);
    beep(6, 784);
    sleep(1);
    beep(6, 659);
    sleep(1);
    beep(6, 523);
}
