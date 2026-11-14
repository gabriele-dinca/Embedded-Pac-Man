#include "logo.h"

#define ROWS 32
#define COLS 32
#define CELL 4


void drawStartLogo(void) {
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    Graphics_fillCircle(&g_sContext, 64, 50, 40);

    Graphics_setFont(&g_sContext, &g_sFontCmss14b);
    Graphics_drawStringCentered(
        &g_sContext,
        (int8_t *)"PAC-MAN",
        AUTO_STRING_LENGTH,
        64, 110,
        TRANSPARENT_TEXT
    );

    int16_t center_y = 50;
    int16_t center_x = 74;

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    int16_t i;
    for (i = 1; i <= 32; i++) {
        Graphics_drawLine(&g_sContext, center_x, center_y + i, center_x, center_y - i);
        center_x++;
    }


}
