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

void PORT2_IRQHandler(void) {
    // Save status and Clear flags
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    // Foreach column pin
    int i;
    for (i = 0; i < PORT_2_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_2_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P2, port_2_columns[i][0]);
            break;
        }
    }
}

void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    // Foreach column pin
    int i;
    for (i = 0; i < PORT_3_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_3_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P3, port_3_columns[i][0]);
            break;
        }
    }
}

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    // Foreach column pin
    int i;
    for (i = 0; i < PORT_4_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_4_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P4, port_4_columns[i][0]);
            break;
        }
    }
}

void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // Foreach column pin
    int i;
    for (i = 0; i < PORT_5_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_5_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P5, port_5_columns[i][0]);
            break;
        }
    }
}

void PORT6_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    // Foreach column pin
    int i;
    for (i = 0; i < PORT_6_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_6_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P6, port_6_columns[i][0]);
            break;
        }
    }
}
