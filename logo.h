#ifndef __LOGO_H__
#define __LOGO_H__

#include <stdint.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

extern Graphics_Context g_sContext;

// (x,y) coordinates and the radius of the circle
#define PAC_MAN_x 64
#define PAC_MAN_y 50
#define PAC_MAN_radius 40

// Draws the initial screen of the application
void drawStartLogo(void);

// Draws the press to play screen
void drawContinue(void);

#endif /* __LOGO_H__ */
