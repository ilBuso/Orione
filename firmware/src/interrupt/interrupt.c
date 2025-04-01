/**
 * @file interrupt.c
 * @brief Implementation of interrupt handlers for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file implements the interrupt handlers
 * declared in interrupt.h.
 */

#include "interrupt.h"

void PORT1_IRQHandler(void) {
    // Save status and Clear flags
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    if (status & GPIO_PIN5) {
        // Handle mute
        UART_transmitData(EUSCI_A2_BASE, /*mute/un-mute keycode*/);
    }
}

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

volatile uint8_t last_channel_a_state = 0;

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);


    if (status & GPIO_PIN3) {
        // debounce

        
        // Get status
        uint8_t channel_a_state = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3);
        uint8_t channel_b_state = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1);
        
        // If Channel A state changed (i.e., it's different from last_channel_a_state)
        if (channelAState != last_channel_a_state) {
            // If Channel A state and Channel B state are different, encoder is rotating clockwise
            // If Channel A state and Channel B state are the same, encoder is rotating counter-clockwise
            if (channel_a_state != channel_b_state) {
                volume++;
            } else {
                volume--;
            }
            
            // Update last Channel A state
            last_channel_a_state = channel_a_state;
        }    
    }

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
