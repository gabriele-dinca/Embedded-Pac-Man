#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdint.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

#define ROWS 16
#define COLS 16
#define CELL 8


enum themeMode_t {
    LIGHT,
    DARK
};

extern Graphics_Context g_sContext;

/*
 * This matrix is designed to resemble the original Pac-Man maze.
 * It is a 16x16 grid, with each cell drawn as an 8x8-pixel square,
 * filling the 128x128-pixel LCD.
 *
 * 1 = wall
 * 0 = corridor
 */
extern uint8_t maze[ROWS][COLS];

void drawMaze(enum themeMode_t theme);
void paintCell(uint8_t x, uint8_t y, int32_t color);


#endif /* __MAZE_H__ */
