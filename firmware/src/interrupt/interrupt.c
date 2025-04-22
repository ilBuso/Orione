/**
 * @file interrupt.c
 * @brief Implementation of interrupt handlers for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 *
 * This file implements the interrupt handlers and initialization function
 * declared in interrupt.h.
 */

#include "interrupt.h"

void PORT1_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

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
            printf("Button 1 pressed (P4.7 → P1.6)\n");
        }

        GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN4);

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7) == 0) {
            printf("Button 1 pressed (P5.4 → P1.6)\n");
        }
    }

    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN7);
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN4);
}
