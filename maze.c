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

// each row { LIGHT, DARK } is associates with an entity
int32_t cellColors[NUM_TYPES][NUM_THEMES] = {
    { GRAPHICS_COLOR_GOLD, GRAPHICS_COLOR_GOLD },     // Coin themes
    { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_BLACK },     // Space themes
    { GRAPHICS_COLOR_GRAY, GRAPHICS_COLOR_BLUE },       // Wall themes
    { GRAPHICS_COLOR_ORANGE, GRAPHICS_COLOR_YELLOW },   // Pac-Man themes
    { GRAPHICS_COLOR_GOLD, GRAPHICS_COLOR_GOLD },       // Power Pellet themes
    { GRAPHICS_COLOR_MAGENTA, GRAPHICS_COLOR_MAGENTA }  // Ghost themes
};


void drawMaze(ThemeMode_t theme) {
    uint8_t row;
    uint8_t col;
    int16_t x0, y0, x1, y1;
    Graphics_Rectangle cell;


    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            x0 = col * CELL;        // left border
            y0 = row * CELL;        // top border
            x1 = x0 + CELL - 1;     // right border
            y1 = y0 + CELL - 1;     // bottom border



            // Colors of the CELL
            Graphics_setForegroundColor(
                &g_sContext,
                cellColors[maze[row][col]][theme]
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


void DrawMaze_New() {
    uint8_t row;
    uint8_t col;
    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            drawCell(row,col, maze[row][col]);
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






