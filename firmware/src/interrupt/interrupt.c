/**
 * @file interrupt.c
 * @brief Implementation of interrupt handlers for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 */

#include "interrupt.h"

// 24 MHz DCO → 24.000.000 tick/s → 50 ms = 24e6 * 0.05 = 1.200.000 tick
#define RELEASE_DEBOUNCE_TICKS 2400000

// Timer32_1 interrupt handler (T32_INT2_IRQHandler for Timer32_1)
void T32_INT2_IRQHandler(void) {
    // Clear the underflow flag
    Timer32_clearInterruptFlag(TIMER32_1_BASE);

    // Stop the timer immediately to prevent continuous interrupts in periodic mode
    Timer32_haltTimer(TIMER32_1_BASE);

    // No new interrupt arrived for 50 ms:
    printf("released\n");
}

void PORT1_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    // FERMA completamente il timer e cancella interrupt pendenti
    Timer32_haltTimer(TIMER32_1_BASE);
    Timer32_clearInterruptFlag(TIMER32_1_BASE);

    // REINIZIALIZZA il timer completamente in modalità periodic per controllare meglio
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_enableInterrupt(TIMER32_1_BASE);

    // Imposta il nuovo conteggio e avvia
    Timer32_setCount(TIMER32_1_BASE, RELEASE_DEBOUNCE_TICKS);
    Timer32_startTimer(TIMER32_1_BASE, false);

    if (status & GPIO_PIN6) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == 0) {
            printf("Button 1 pressed (P4.7 → P1.6)\n");
        }

        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == 0) {
            printf("Button 1 pressed (P5.4 → P1.6)\n");
        }
    }

    else if (status & GPIO_PIN7) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7) == 0) {
            printf("Button 1 pressed (P4.7 → P1.7)\n");
        }

        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7) == 0) {
            printf("Button 1 pressed (P5.4 → P1.7)\n");
        }
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
}