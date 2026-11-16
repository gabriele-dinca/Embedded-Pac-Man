/*
 * pac_man.c
 *
 *  Created on: 16 Nov 2026
 *      Author: gabriele_dinca
 */
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include <stdint.h>

#include "pac_man.h"
#include "maze.h"

#define X 0
#define Y 1

/* ADC results buffer */
static uint16_t resultsBuffer[2];

typedef enum {
    DIR_NONE,
    DIR_TOP,
    DIR_RIGHT,
    DIR_BOTTOM,
    DIR_LEFT
} Direction;

volatile Direction currDirection = DIR_NONE;
volatile uint8_t makeMove = 0;

/* ADC results buffer */
static uint16_t resultsBuffer[2];

void _adcInit(){
    /* Configures Pin 6.0 and 4.4 as ADC input */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

        /* Initializing ADC (ADCOSC/64/8) */
        ADC14_enableModule();
        ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

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

#define TIMER_PERIOD 5000

/* Timer_A UpMode Configuration Parameter */
const Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_ACLK,              // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,         // SMCLK/1 = 3MHz
        TIMER_PERIOD,                           // every half second
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
    Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_TA1_0);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();
}

Pac_Man_t pac_man = {13, 8};

void movePac_Man() {

    // Pacman position before moving
    uint8_t curr_x = pac_man.x;
    uint8_t curr_y = pac_man.y;

    switch (currDirection) {
        case DIR_TOP: // TOP
        //printf("TOP\n");

        if (curr_x - 1 >= 0 && maze[curr_x - 1][curr_y] == 0) {
            maze[curr_x][curr_y] = 0;
            maze[curr_x - 1][curr_y] = 2;
            pac_man.x--;
        } else return;
        break;

        case DIR_BOTTOM:  // BOTTOM
        //printf("BOTTOM\n");

        if (curr_x + 1 < 16 && maze[curr_x + 1][curr_y] == 0) {
            maze[curr_x][curr_y] = 0;
            maze[curr_x + 1][curr_y] = 2;
            pac_man.x++;
        } else return;
        break;

        case DIR_RIGHT:  // RIGHT
        //printf("RIGHT\n");

        if (curr_y + 1 < 16 && maze[curr_x][curr_y + 1] == 0) {
            maze[curr_x][curr_y] = 0;
            maze[curr_x][curr_y + 1] = 2;
            pac_man.y++;
        } else return;
        break;

        case DIR_LEFT: // LEFT
        //printf("LEFT\n");

        if (curr_y - 1 >= 0 && maze[curr_x][curr_y - 1] == 0) {
            maze[curr_x][curr_y] = 0;
            maze[curr_x][curr_y - 1] = 2;
            pac_man.y--;
        } else return;
        break;

    }

    // new pac-man position
    paintCell(pac_man.x, pac_man.y, GRAPHICS_COLOR_YELLOW);
    // old pac-man position
    paintCell(curr_x, curr_y, GRAPHICS_COLOR_BLACK);


}




void playGame(void) {
    _adcInit();
    gameTimer_config();

    while(1) {
        PCM_gotoLPM0();

        /*if (makeMove == 1) {
            makeMove = 0;

            movePac_Man();
        }*/
    }
}





void TA1_0_IRQHandler(void) {
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    //makeMove = 1;
    movePac_Man();
}




/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1) {

        /* Store ADC14 conversion results */
        resultsBuffer[X] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[Y] = ADC14_getResult(ADC_MEM1);

        if ((resultsBuffer[X] >= 6000 && resultsBuffer[X] <= 10000) &&
            (resultsBuffer[Y] >= 6000 && resultsBuffer[Y] <= 10000)) {
            //printf("Returning\n");
            return;
        }


        if (resultsBuffer[Y] > 13000) { // TOP
            printf("TOP\n");
            currDirection = DIR_TOP;
        } else if (resultsBuffer[Y] < 3000) { // BOTTOM
            printf("BOTTOM\n");
            currDirection = DIR_BOTTOM;
        } else if (resultsBuffer[X] > 13000) { // RIGHT
            printf("RIGHT\n");
            currDirection = DIR_RIGHT;
        } else if (resultsBuffer[X] < 3000) { // LEFT
            printf("LEFT\n");
            currDirection = DIR_LEFT;
        }
    }
}




