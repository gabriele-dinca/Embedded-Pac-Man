#ifndef __MAZE_H__
#define __MAZE_H__

#include <stdint.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// ************************ Definitions ************************

#define ROWS 16     // number of rows for the maze matrix
#define COLS 16     // number of columns for the maze matrix
#define CELL 8      // dimension of a single entry of the matrix (expressed in pixels)

#define LOOP_HOLE_X 7              // X coordinate of both left and right loophole
#define LOOP_HOLE_LEFT_Y 0         // Y coordinate of the left loophole
#define LOOP_HOLE_RIGHT_Y 15       // Y coordinate of the right loophole


// ************************ Data Structures ************************


// Defines the available themes
typedef enum {
    LIGHT,      // 0=light
    DARK,       // 1=dark
    NUM_THEMES
} ThemeMode_t;

// Defines the available types of cell which can be found on the matrix
typedef enum {
    COIN,  // 0=coin
    SPCE,  // 1=space
    WALL,  // 2=wall
    PACM,  // 3=pac_man
    POWP,   // 4=power pellet
    GHST,  // 5=ghost
    NUM_TYPES
} CellType_t ;

extern int32_t cellColors[NUM_TYPES][NUM_THEMES];

/*
 * This matrix is designed to resemble the original Pac-Man maze.
 * It is a 16x16 grid, with each cell drawn as an 8x8-pixel square,
 * filling the 128x128-pixel LCD.
 */
extern CellType_t maze[ROWS][COLS];

extern Graphics_Context g_sContext;

// ************************ Functions ************************

/*
 * Initialize the maze using a specified theme
 *
 * Params:
 *     - theme: colours to be used
 */
void drawMaze(ThemeMode_t theme);


/*
 * Colors the specified cell of the display using the specified color
 *
 * Params:
 *     - x: row of the maze matrix
 *     - y: column of the maze matrix
 *     - color: paint to be applied to the 8x8 cell
 */
void paintCell(uint8_t x, uint8_t y, int32_t color);


#endif /* __MAZE_H__ */
