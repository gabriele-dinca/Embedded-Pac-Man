#include "maze.h"

uint8_t maze[ROWS][COLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,0,0,0,1,0,0,0,1},
    {1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,1,0,1,1,1,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,1,0,1,0,0,1,0,1,1,1,1,1},
    {0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0},
    {1,1,1,1,1,0,1,1,0,1,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1},
    {1,0,1,1,1,1,0,1,0,1,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,1,1,0,1,1,1,1,1,1,1,0,1,0,1},
    {1,0,0,0,0,0,0,0,2,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};



void drawMaze(enum themeMode_t theme) {
    uint8_t row;
    uint8_t col;
    int16_t x0, y0, x1, y1;
    Graphics_Rectangle cell;

    int32_t corridor_color;


    // Theme Management
    switch(theme) {
    case LIGHT:
        corridor_color = GRAPHICS_COLOR_WHITE;
        break;
    case DARK:
        corridor_color = GRAPHICS_COLOR_BLACK;
        break;
    default:
        corridor_color = GRAPHICS_COLOR_BLACK;
        break;
    }

    int32_t colorApplied;


    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            x0 = col * CELL;        // left border
            y0 = row * CELL;        // top border
            x1 = x0 + CELL - 1;     // right border
            y1 = y0 + CELL - 1;     // bottom border

            switch(maze[row][col]) {
               case 0:
                   colorApplied = corridor_color;
                   break;
               case 1:
                   colorApplied = GRAPHICS_COLOR_BLUE;
                   break;
               case 2:
                   colorApplied = GRAPHICS_COLOR_YELLOW;
            }

            // Colors of the CELL
            Graphics_setForegroundColor(
                &g_sContext,
                colorApplied
            );

            // Draw the CELL on the LCD screen
            cell.xMin = x0;
            cell.yMin = y0;
            cell.xMax = x1;
            cell.yMax = y1;
            Graphics_fillRectangle(&g_sContext, &cell);
        }
    }
}


void paintCell(uint8_t x, uint8_t y, int32_t color) {

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

}









