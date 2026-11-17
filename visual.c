/*
 * visual.c
 *
 *  Created on: 17 Nov 2026
 *      Author: gabriele_dinca
 */

#include "pac_man.h"
#include <stdint.h>
#include <ti/grlib/grlib.h>

#define SPRITE_SIZE 8

extern Graphics_Context g_sContext;

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

typedef struct {
    int32_t primary;
    int32_t secondary;
} ColorPair_t;

// each row { PRIMARY, SECONDARY } is associates with an cell type
ColorPair_t cellColors1[NUM_TYPES] = {
    { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_GOLD },          // 0=Coin
    { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_BLACK },         // 1=Space
    { GRAPHICS_COLOR_BLUE, GRAPHICS_COLOR_BLUE },           // 2=Wall
    { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_YELLOW },        // 3=Pac-Man
    { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_GOLD },          // 4=Power Pellet
    { GRAPHICS_COLOR_BLACK, GRAPHICS_COLOR_MAGENTA }        // 5=Ghost
};



static const uint8_t pacManBitmap[NUM_PAC_DIRECTIONS][SPRITE_SIZE][SPRITE_SIZE] = {

   [PAC_UP] = {
           {0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0},
           {1, 0, 0, 0, 0, 0, 1, 0},
           {1, 0, 0, 0, 0, 0, 1, 0},
           {1, 1, 0, 0, 0, 1, 1, 0},
           {1, 1, 1, 0, 1, 1, 1, 0},
           {0, 1, 1, 1, 1, 1, 0, 0},
           {0, 0, 1, 1, 1, 0, 0, 0}
      },

   [PAC_DOWN] = {
           {0, 0, 0, 1, 1, 1, 0, 0},
           {0, 0, 1, 1, 1, 1, 1, 0},
           {0, 1, 1, 1, 0, 1, 1, 1},
           {0, 1, 1, 0, 0, 0, 1, 1},
           {0, 1, 0, 0, 0, 0, 0, 1},
           {0, 1, 0, 0, 0, 0, 0, 1},
           {0, 0, 0, 0, 0, 0, 0, 0},
           {0, 0, 0, 0, 0, 0, 0, 0}
    },


    [PAC_RIGHT] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    },

    [PAC_LEFT] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 1, 1},
        {0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    },

    [PAC_NONE] = {
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 1, 1, 1, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 0},
            {1, 1, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 0, 0, 0},
            {0, 1, 1, 1, 0, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0}
        }



};







static const uint8_t bitmaps[NUM_TYPES][SPRITE_SIZE][SPRITE_SIZE] = {
    [COIN] = {
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 1, 1, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 0, 0}
     },

    [SPCE] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    },

    [WALL] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    },

    [PACM] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    },


    [POWP] = {
        {0, 0, 0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0, 0, 0}
    },

    [GHST] = {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 0, 1, 1, 0, 1, 1},
        {1, 0, 0, 1, 1, 0, 0, 1}
    }
};



void drawPacMan(uint8_t x, uint8_t y, PacDirection_t dir) {
    uint8_t i, j;

    for (i = 0; i < SPRITE_SIZE; i++) {
        for (j = 0; j < SPRITE_SIZE; j++) {
             if (pacManBitmap[dir][i][j] == 0) {
                 Graphics_setForegroundColor(&g_sContext, cellColors1[PACM].primary);
             } else {
                 Graphics_setForegroundColor(&g_sContext, cellColors1[PACM].secondary);
             }

             Graphics_drawPixel(&g_sContext,
                                y * SPRITE_SIZE + j,
                                x * SPRITE_SIZE + i);
        }
    }
}


void drawCell(uint8_t x, uint8_t y, CellType_t type) {
    uint8_t i, j;

    for (i = 0; i < SPRITE_SIZE; i++) {
        for (j = 0; j < SPRITE_SIZE; j++) {
             if (bitmaps[type][i][j] == 0) {
                 Graphics_setForegroundColor(&g_sContext, cellColors1[type].primary);
             } else {
                 Graphics_setForegroundColor(&g_sContext, cellColors1[type].secondary);
             }

             Graphics_drawPixel(&g_sContext,
                                y * SPRITE_SIZE + j,
                                x * SPRITE_SIZE + i);
        }
    }
}







