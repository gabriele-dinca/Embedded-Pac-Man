#include <stdint.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef struct {
    uint16_t frequency;     // Hz; 0 = pause
    uint16_t duration_ms;   // length of the note
} Note;

static const Note melody[] = {
    {659, 100}, {0, 40}, {784, 100}, {0, 40}, {880, 180},
    {784, 100}, {0, 40}, {659, 100}, {0, 40}, {523, 180},
    {587, 100}, {659, 100}, {784, 220},
    {0, 120}
};

/*
 * Il tuo setup usa DCO a 3 MHz e SMCLK = DCO.
 * Il timer divide SMCLK per 12, quindi conta a 250 kHz.
 */
#define TIMER_CLOCK_HZ 250000UL

static void buzzerInit(void) {

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_3);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // P2.7 è TA0.4 sul setup MSP432P401R indicato nel sample.
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_12,
        1,  // placeholder period; it gets update for every note
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_DO_CLEAR
    };

    Timer_A_CompareModeConfig compareConfig = {
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
        TIMER_A_OUTPUTMODE_RESET_SET,
        1   // placeholder duty cycle; it gets update for every note
    };

    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
}

static void playTone(uint16_t frequency)
{
    if (frequency == 0) {
        Timer_A_stopTimer(TIMER_A0_BASE);
        GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);
        return;
    }

    uint32_t period = TIMER_CLOCK_HZ / frequency;

    Timer_A_stopTimer(TIMER_A0_BASE);

    Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_12,
        (uint16_t)(period - 1),
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_DO_CLEAR
    };

    Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);

    // CCR4 deve stare dentro il periodo; metà periodo dà duty circa 50%.
    Timer_A_setCompareValue(
        TIMER_A0_BASE,
        TIMER_A_CAPTURECOMPARE_REGISTER_4,
        (uint16_t)(period / 4));

    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

static void delayMs(uint16_t duration_ms)
{
    while (duration_ms--) {
        __delay_cycles(3000); // circa 1 ms con MCLK a 3 MHz
    }
}

void playMelody(void) {
    // Initialise piezo buzzer
    buzzerInit();

    uint16_t i;
    for (i = 0; i < sizeof(melody) / sizeof(Note); i++) {
        playTone(melody[i].frequency);
        delayMs(melody[i].duration_ms);
    }

}
