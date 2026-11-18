#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "HAL_OPT3001.h"
#include <stdio.h>

#include "state_machine.h"
#include "maze.h"
//#include "melody.h"
#include "logo.h"
#include "visual.h"

State_t current_state = STATE_INIT;
StateMachine_t fsm[] = {
   {STATE_INIT, fn_INIT},
   {STATE_PLAY, fn_PLAY}
};



// Light sensor config
#define themeThreshold 200.0f
Theme_t theme;

void fn_INIT() {
    current_state = STATE_INIT;
    // Draw Pac-Man picture on startup
    drawStartLogo();

    // Obtain lux value from light sensor
    float lux = OPT3001_getLux();
    printf("lux: %f\n", lux);

    // Select theme mode based on the sensed value
    if (lux < themeThreshold) {
        theme = DARK;
    } else {
        theme = LIGHT;
    }

    // Play melody
    // playMelody();

    // Draw information to continue
    drawContinue();

}

void fn_PLAY() {
    current_state = STATE_PLAY;
    //drawMaze(theme);
    drawMaze(theme);

    playGame();

}




void state_machine(void) {

    // Set P5.1 as input pin
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1);


    // Interrupt_enableSleepOnIsrExit();
    Interrupt_enableInterrupt(INT_PORT5);

    /* Enabling MASTER interrupts */
    Interrupt_enableMaster();

    // Initialize the game
    fn_INIT();

    /* Sleeping when not in use */
    while (1)
    {
        PCM_gotoLPM0();
        if (current_state < NUM_STATES){
            (*fsm[current_state].state_function)();
        }
        else{
            /* serious error */
        }
    }
}


// Interrupt handler
void PORT5_IRQHandler(void) {
    /* Check which pins generated the interrupts */
    uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

    // clear interrupt flag
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // check if we received P5.1 (button S1 pressed)
    if((status & GPIO_PIN1)){
        current_state = STATE_PLAY;
    }

}
