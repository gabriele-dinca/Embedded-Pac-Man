#include "logo.h"


void drawStartLogo(void) {
    // Sets BLACK background
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);

    // Draws a YELLOW circle
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_fillCircle(&g_sContext, PAC_MAN_x, PAC_MAN_y, PAC_MAN_radius);

    // Draws PAC-MAN mouth
    int center_x = PAC_MAN_x + 10;
    int center_y = PAC_MAN_y;

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    int i;
    for (i = 1; i <= 32; i++) {
        Graphics_drawLine(&g_sContext, center_x, center_y + i, center_x, center_y - i);
        center_x++;
    }

    // Writes "PAC-MAN" using the specified font
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_setFont(&g_sContext, &g_sFontCmss14b);
    Graphics_drawStringCentered(
        &g_sContext,
        (int8_t *)"PAC-MAN",
        AUTO_STRING_LENGTH,
        64, 110,
        TRANSPARENT_TEXT
    );
}


void drawContinue(void) {
    // Sets BLACK background
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);


    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_setFont(&g_sContext, &g_sFontCmss14b);
    Graphics_drawStringCentered(
        &g_sContext,
        (int8_t *)"Press S1 to play",
        AUTO_STRING_LENGTH,
        64, 64,
        TRANSPARENT_TEXT);
}
