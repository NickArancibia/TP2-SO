#include <stdint.h>

// Includes del juego de eliminador
#ifndef __ELIMINATOR_GAME_H__
#define __ELIMINATOR_GAME_H__

#define ELIMINATOR_EXEC         1
#define TICKS_PER_FRAME         1
#define MS_PER_FRAME            180000

#define BLOCKED                 1
#define FREE                    0

#define SNAKE_HEAD_SIZE         8
#define WALL_SIZE               8
#define LETTERS_THICK           8

#define WIDTH                   128 // x 8 = 1024
#define HEIGHT                  96 // x 8 = 768

#define ELIMINATOR_FONT_LEVEL   6
#define PIXELS_WIDTH            1024
#define PIXELS_HEIGHT           768

#define CRASHED                 1

#define P1_UP_KEY           sysupArrowValue()
#define P1_DOWN_KEY         sysdownArrowValue()
#define P1_LEFT_KEY         sysleftArrowValue()
#define P1_RIGHT_KEY        sysrightArrowValue()

#define P2_UP_KEY           'w'
#define P2_DOWN_KEY         's'
#define P2_LEFT_KEY         'a'
#define P2_RIGHT_KEY        'd'


#define ESC                 27
#define SPACE               ' '
#define ONE_PLAYER          '1'
#define TWO_PLAYERS         '2'
#define COMPUTER            '3'
#define RESET               'r'
#define A_OPTION            'a'
#define B_OPTION            'b'
#define C_OPTION            'c'


enum Direction {UP, DOWN, LEFT, RIGHT};
enum Player {CPU, HUMAN};


typedef struct {
    uint8_t x;
    uint8_t y;
    uint32_t color;
} SnakeHead;

void eliminatorGame(void);
void playAlone(void);
void playTwoPlayers(int player2);
void cleanBoard(void);
void printWall(void);

#endif