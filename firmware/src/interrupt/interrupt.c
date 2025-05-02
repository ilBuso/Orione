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

#include "interrupt.h"

/** @brief Key code for mute toggle */
#define MUTE_KEYCODE 127

/** @brief Key code for volume up */
#define VOLUME_UP_KEYCODE 128

/** @brief Key code for volume down */
#define VOLUME_DOWN_KEYCODE 129

/** @brief Maximum volume value */
#define MAX_VOLUME 100

/** @brief Debounce delay cycles for encoder */
#define DEBOUNCE_DELAY 1000

void PORT1_IRQHandler(void) {
    // Save status and Clear flags
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    if (status & GPIO_PIN5) {
        //Debounce using a delay

        __delay_cycles(DEBOUNCE_DELAY);

        //Check if button is still pressed
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW) {
            //Handle mute by sending mute keycode
            UART_transmitData(EUSCI_A2_BASE, MUTE_KEYCODE);

            //Toggle LED for visual feedback
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
        }
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
volatile uint8_t prev_volume = 0;

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);


    if (status & GPIO_PIN3) {
        // debounce
        
        __delay_cycles(DEBOUNCE_DELAY);
        
        // Get status
        uint8_t channel_a_state = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3);
        uint8_t channel_b_state = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN1);
        
        // If Channel A state changed (i.e., it's different from last_channel_a_state)
        if (channel_a_state != last_channel_a_state) {
            // If Channel A state and Channel B state are different, encoder is rotating clockwise
            // If Channel A state and Channel B state are the same, encoder is rotating counter-clockwise
            if (channel_a_state != channel_b_state) {
                //Increase volume
                if(volume < MAX_VOLUME){
                    valume ++;
                    UART_transmitData(EUSCI_A2_BASE, VOLUME_UP_KEYCODE);
                }
            } else {
                if(volume > 0){
                    //Decrease volume
                    volume --;
                    UART_transmitData(EUSCI_A2_BASE, VOLUME_DOWN_KEYCODE);
                }
            }

            //If volume changed by a sufficient amount, send the actual volume value
            if(abs(volume - prev_volume) >= 5){
                UART_transmitData(EUSCI_A2_BASE, volume);
                prev_volume = volume;

                GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
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
