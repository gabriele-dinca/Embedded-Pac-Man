#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "pac_man.h"
#include "maze.h"

#define X 0
#define Y 1

/* ADC results buffer */
static uint16_t XY_Movement[2];



volatile PacDirection_t currDirection = PAC_NONE;
volatile bool makeMove = false;

extern ThemeMode_t theme;

/* ADC results buffer */
static uint16_t XY_Movement[2];

void _adcInit(){
    /* Configures Pin 6.0 and 4.4 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                                   GPIO_PIN0,
                                                   GPIO_TERTIARY_MODULE_FUNCTION);

        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                                   GPIO_PIN4,
                                                   GPIO_TERTIARY_MODULE_FUNCTION);

        /* Initializing ADC (ADCOSC/64/8) */
        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC,
                         ADC_PREDIVIDER_64,
                         ADC_DIVIDER_8,
                         0);

        /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
             * with internal 2.5v reference */
        ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);

        ADC14_configureConversionMemory(ADC_MEM0,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

        ADC14_configureConversionMemory(ADC_MEM1,
                                        ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                        ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

        /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
         *  is complete and enabling conversions */
        ADC14_enableInterrupt(ADC_INT1);

        /* Enabling Interrupts */
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();

        /* Setting up the sample timer to automatically step through the sequence
         * convert.
         */
        ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

        /* Triggering the start of the sample */
        ADC14_enableConversion();
        ADC14_toggleConversionTrigger();
}


/*
 * ACLK = 32,768 kHz = 32768 Hz
 * This means 32768 increments per second
 * TIMER_PERIOD / 32768 = REFRESH_RATE
 * */
#define TIMER_PERIOD 4916

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_ACLK,               // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // ACLK/1 = 32,768 kHz
        TIMER_PERIOD,                           // Configured above
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE ,    // Enable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
};

void gameTimer_config(void) {
    /* Configuring P1.0 as output */
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /* Configuring Timer_A1 for Up Mode */
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    /* Enabling interrupts and starting the timer */
    //Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();
}

// Pac-Man initial position on the maze matrix
Pac_Man_t pac_man = { START_X, START_Y };

void movePacMan(void) {

    // Pac-Man position before moving
    uint8_t curr_x = pac_man.x;
    uint8_t curr_y = pac_man.y;

    // Decide whether to apply a new coloring to the matrix
    bool applyMove = false;

    switch (currDirection) {
        case PAC_UP:

            if (curr_x - 1 >= 0 &&
               (maze[curr_x - 1][curr_y] == SPCE ||
                maze[curr_x - 1][curr_y] == COIN)) { // UP

                maze[curr_x][curr_y] = SPCE;
                maze[curr_x - 1][curr_y] = PACM;
                pac_man.x--;

                applyMove = true;
            }
            break;

        case PAC_DOWN:

            if (curr_x + 1 < 16 &&
               (maze[curr_x + 1][curr_y] == SPCE ||
                maze[curr_x + 1][curr_y] == COIN)) { // DOWN

                maze[curr_x][curr_y] = SPCE;
                maze[curr_x + 1][curr_y] = PACM;
                pac_man.x++;

                applyMove = true;
            }
            break;

        case PAC_RIGHT:

            if (curr_y + 1 < 16 &&
               (maze[curr_x][curr_y + 1] == SPCE ||
                maze[curr_x][curr_y + 1] == COIN)) { // RIGHT

                maze[curr_x][curr_y] = SPCE;
                maze[curr_x][curr_y + 1] = PACM;
                pac_man.y++;

                applyMove = true;
            } else if(curr_x == LOOP_HOLE_X && curr_y == LOOP_HOLE_RIGHT_Y) { // Manages loop hole right
                maze[curr_x][curr_y] = SPCE;
                maze[curr_x][LOOP_HOLE_LEFT_Y] = PACM;
                pac_man.y = LOOP_HOLE_LEFT_Y;

                applyMove = true;
            }
            break;

        case PAC_LEFT:

            if (curr_y - 1 >= 0 &&
               (maze[curr_x][curr_y - 1] == SPCE ||
                maze[curr_x][curr_y - 1] == COIN)) { // LEFT

                maze[curr_x][curr_y] = SPCE;
                maze[curr_x][curr_y - 1] = PACM;
                pac_man.y--;
                applyMove = true;
            } else if (curr_x == LOOP_HOLE_X && curr_y == LOOP_HOLE_LEFT_Y) { // Manages left loop hole
                maze[curr_x][curr_y] = SPCE;
                maze[curr_x][LOOP_HOLE_RIGHT_Y] = PACM;
                pac_man.y = LOOP_HOLE_RIGHT_Y;

                applyMove = true;
            }
            break;

        case PAC_NONE:
            break;
    }

    /* If it was possible to move pac-man in one of the four possible direction,
     * pac-man is drawn on the new position.
     */
    if (applyMove) {

        // new pac-man position
        // paintCell(pac_man.x, pac_man.y, cellColors[PACM][theme]);
        drawPacMan(pac_man.x, pac_man.y, currDirection);

        // old pac-man position
        // paintCell(curr_x, curr_y, cellColors[SPCE][theme]);
        drawCell(curr_x, curr_y, SPCE);
    }

}




void playGame(void) {
    _adcInit();
    gameTimer_config();

    while(1) {
        PCM_gotoLPM0();

        if (currDirection != PAC_NONE && makeMove) {
            makeMove = false;
            movePacMan();
        }
    }
}




// Timer A1 Handler for gameTick
void TA1_0_IRQHandler(void) {
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    makeMove = true;
}




/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in XY_Movement */
void ADC14_IRQHandler(void) {
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1) {

        /* Store ADC14 conversion results */
        XY_Movement[X] = ADC14_getResult(ADC_MEM0);
        XY_Movement[Y] = ADC14_getResult(ADC_MEM1);

        if ((XY_Movement[X] >= 4000 && XY_Movement[X] < 10000) &&
            (XY_Movement[Y] >= 4000 && XY_Movement[Y] < 10000)) {
            // No action is needed
            return;
        }


        if (XY_Movement[Y] >= 10000) { // UP
            currDirection = PAC_UP;

        } else if (XY_Movement[Y] < 4000) { // DOWN
            currDirection = PAC_DOWN;

        } else if (XY_Movement[X] >= 10000) { // RIGHT
            currDirection = PAC_RIGHT;

        } else if (XY_Movement[X] < 4000) { // LEFT
            currDirection = PAC_LEFT;

        }
    }
}




