#include "maze.h"


CellType_t maze[ROWS][COLS] = {
    {WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL},
    {WALL,GHST,POWP,SPCE,WALL,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,WALL,SPCE,SPCE,SPCE,WALL},
    {WALL,SPCE,WALL,SPCE,WALL,SPCE,WALL,WALL,WALL,WALL,SPCE,WALL,SPCE,WALL,SPCE,WALL},
    {WALL,SPCE,WALL,SPCE,COIN,SPCE,SPCE,SPCE,SPCE,SPCE,COIN,SPCE,SPCE,WALL,SPCE,WALL},
    {WALL,SPCE,WALL,WALL,WALL,SPCE,WALL,WALL,WALL,WALL,SPCE,WALL,WALL,WALL,SPCE,WALL},
    {WALL,SPCE,SPCE,SPCE,SPCE,SPCE,WALL,SPCE,SPCE,WALL,SPCE,SPCE,SPCE,SPCE,SPCE,WALL},
    {WALL,WALL,WALL,WALL,WALL,SPCE,WALL,SPCE,SPCE,WALL,SPCE,WALL,WALL,WALL,WALL,WALL},
    {SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE},
    {WALL,WALL,WALL,WALL,WALL,SPCE,WALL,WALL,SPCE,WALL,SPCE,WALL,WALL,WALL,WALL,WALL},
    {WALL,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,WALL,SPCE,WALL,SPCE,SPCE,SPCE,SPCE,SPCE,WALL},
    {WALL,SPCE,WALL,WALL,WALL,WALL,SPCE,WALL,SPCE,WALL,SPCE,WALL,WALL,WALL,SPCE,WALL},
    {WALL,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,WALL},
    {WALL,SPCE,WALL,WALL,SPCE,WALL,WALL,WALL,WALL,WALL,WALL,WALL,SPCE,WALL,SPCE,WALL},
    {WALL,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,PACM,SPCE,SPCE,SPCE,SPCE,SPCE,SPCE,WALL},
    {WALL,WALL,WALL,SPCE,WALL,WALL,SPCE,SPCE,SPCE,SPCE,WALL,WALL,SPCE,WALL,WALL,WALL},
    {WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL,WALL}
};



void drawMaze(Theme_t theme) {
    int row;
    int col;
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            drawCell(row, col, theme, maze[row][col]);
        }
    }
}

// TODO: to be deleted
/*void paintCell(uint8_t x, uint8_t y, int32_t color) {

    int16_t x0, y0, x1, y1;
    Graphics_Rectangle cell;

    x0 = y * CELL;          // left border
    y0 = x * CELL;          // top border
    x1 = x0 + CELL - 1;     // right border
    y1 = y0 + CELL - 1;     // bottom border


    // Colors of the CELL
    Graphics_setForegroundColor(&g_sContext, color);

    // Draw the CELL on the LCD screen
    cell.xMin = x0;
    cell.yMin = y0;
    cell.xMax = x1;
    cell.yMax = y1;
    Graphics_fillRectangle(&g_sContext, &cell);

}*/






