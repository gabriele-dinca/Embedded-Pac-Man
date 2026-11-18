#ifndef __VISUAL_H__
#define __VISUAL_H__


#define SPRITE_SIZE 8

#include "pac_man.h"

// Graphic context
extern Graphics_Context g_sContext;

// Defines the available themes
typedef enum {
    LIGHT,      // 0=light
    DARK,       // 1=dark
    NUM_THEMES
} Theme_t;

// Defines the available types of cell which can be found on the matrix
typedef enum {
    COIN,  // 0=coin
    SPCE,  // 1=space
    WALL,  // 2=wall
    PACM,  // 3=pac_man
    POWP,  // 4=power pellet
    GHST,  // 5=ghost
    NUM_TYPES
} CellType_t ;

// Define a primary and secondary color
typedef struct {
    int32_t primary;
    int32_t secondary;
} CellColors_t;

// { PRIMARY, SECONDARY } is associates with a specific theme and cell type
static const CellColors_t cellColors[NUM_THEMES][NUM_TYPES] = {
   [LIGHT] = {
       [COIN] = { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_GOLD },
       [SPCE] = { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_WHITE },
       [WALL] = { GRAPHICS_COLOR_GRAY, GRAPHICS_COLOR_GRAY },
       [PACM] = { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_ORANGE },
       [POWP] = { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_GOLD },
       [GHST] = { GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_BLUE }
   },

    [DARK] = {
        [COIN] = { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_GOLD },
        [SPCE] = { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_BLACK },
        [WALL] = { GRAPHICS_COLOR_BLUE, GRAPHICS_COLOR_BLUE },
        [PACM] = { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_YELLOW },
        [POWP] = { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_GOLD },
        [GHST] = { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_MAGENTA }
    }

};

/*
 * Draws pac-man with the right orientation in the specified cell.
 *
 * Params:
 *      - x: row of the maze matrix
 *      - y: col of the maze matrix
 *      - theme: colour palette to be used
 *      - dir: select the correct orientation of pac-man's mouth
 */
void drawPacMan(uint8_t x, uint8_t y, Theme_t theme, PacDirection_t dir);


/*
 * Draws a specified figure in the specified cell.
 *
 * Params:
 *      - x: row of the maze matrix
 *      - y: col of the maze matrix
 *      - theme: colour palette to be used
 *      - type: selects the appropriate bitmap
 */
void drawCell(uint8_t x, uint8_t y, Theme_t theme, CellType_t type);




#endif /* __VISUAL_H__ */
