// PAC-MAN Main

// HW dependent libs
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"

// STD libs
#include <stdio.h>
#include <stdint.h>


// Business logic libs
// TODO: maze.h

#define themeMode_Threshold 200.0f
enum themeMode { DARK, LIGHT };

/* Graphic library context */
Graphics_Context g_sContext;

void _lightSensorInit()
{
    /* Initialize I2C communication */
    Init_I2C_GPIO();
    I2C_init();

    /* Initialize OPT3001 digital ambient light sensor */
    OPT3001_init();

    __delay_cycles(100000);

}

void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);

}



void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Graphic Init
    _graphicsInit();

    // Light sensor Init
    _lightSensorInit();
}





#define ROWS 16
#define COLS 16
#define CELL 8

/*
 * This matrix is designed to resemble the original Pac-Man maze.
 * It is a 16x16 grid, with each cell drawn as an 8x8-pixel square,
 * filling the 128x128-pixel LCD.
 *
 * 1 = wall
 * 0 = corridor
 */
const uint8_t maze[ROWS][COLS] = {
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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,0,1,1,0,0,0,0,1,1,0,1,1,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};



void drawMaze(enum themeMode theme)
{
    uint8_t row;
    uint8_t col;
    int16_t x0, y0, x1, y1;
    Graphics_Rectangle cell;

    int32_t corridor_color;


    // Theme Management
    switch(theme) {
    case DARK:
        corridor_color = GRAPHICS_COLOR_WHITE;
        break;
    case LIGHT:
        corridor_color = GRAPHICS_COLOR_BLACK;
        break;
    default:
        corridor_color = GRAPHICS_COLOR_BLACK;
        break;
    }

    for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
            x0 = col * CELL;        // left border
            y0 = row * CELL;        // top border
            x1 = x0 + CELL - 1;     // right border
            y1 = y0 + CELL - 1;     // bottom border

            // Colors of the CELL
            Graphics_setForegroundColor(
                &g_sContext,
                maze[row][col] ? GRAPHICS_COLOR_BLUE : corridor_color
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



/*
 * Main function
 */
int main(void)
{

    _hwInit();


    /* Obtain lux value from OPT3001 */
    float lux = OPT3001_getLux();
    printf("lux: %f\n", lux);

    enum themeMode theme;
    if (lux < themeMode_Threshold) {
        theme = DARK;
    } else {
        theme = LIGHT;
    }

    drawMaze(theme);

    while(1) // infinite loop
    {
    }



}
