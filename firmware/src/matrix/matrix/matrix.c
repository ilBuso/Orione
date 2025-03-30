/**
 * @file matrix.c
 * @brief Implementation of matrix helper functions
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file implements the functions declared in matrix.h
 */

#include "matrix.h"

#include "../../message/message.h"

#include <stdlib.h>

// Helper function to find column index
int8_t get_column_index(uint_fast16_t port, uint_fast16_t pin) {
    const uint8_t (*column_array)[2];
    uint8_t array_size = 0;

    switch(port) {
        case GPIO_PORT_P2:
            column_array = port_2_columns;
            array_size = PORT_2_NUM_PINS;
            break;
        case GPIO_PORT_P3:
            column_array = port_3_columns;
            array_size = PORT_3_NUM_PINS;
            break;
        case GPIO_PORT_P4:
            column_array = port_4_columns;
            array_size = PORT_4_NUM_PINS;
            break;
        case GPIO_PORT_P5:
            column_array = port_5_columns;
            array_size = PORT_5_NUM_PINS;
            break;
        case GPIO_PORT_P6:
            column_array = port_6_columns;
            array_size = PORT_6_NUM_PINS;
            break;
        default:
            return -1;
    }

    int i;
    for (i = 0; i < array_size; i++) {
        if(column_array[i][0] == pin) {
            return column_array[i][1];
        }
    }

    return -1;
}

void scan_rows(uint_fast16_t port, uint_fast16_t pin) {
    // Get column index for the triggered pin
    int8_t column = get_column_index(port, pin);
    if (column < 0) return;

    // Disable interrupts for pins 1 and 2 on Port 1
    GPIO_disableInterrupt(port, pin);

    // Set all rows low
    int i;
    for (i = 0; i < NUM_ROWS; i++) {
        GPIO_setOutputLowOnPin(row_port[i], row_pins[i]);
    }

    // Check each row
    int row;
    for (row = 0; row < NUM_ROWS; row++) {
        // Set current row high
        GPIO_setOutputHighOnPin(row_port[row], row_pins[row]);

        // Brief delay to stabilize
        //__delay_cycles(10);

        // Check if column is still high (key is at this row)
        if(GPIO_getInputPinValue(port, pin) == GPIO_INPUT_PIN_HIGH) {
            
            Fragment* x_ptr = new_fragment(X, row);
            Fragment* y_ptr = new_fragment(Y, column);
            Fragment* info_ptr = new_fragment(INFO, 0); // to fix when we know how we send info

            if (x_ptr == NULL || y_ptr == NULL || info_ptr == NULL) {
                if (x_ptr != NULL) free(x_ptr);
                if (y_ptr != NULL) free(y_ptr);
                if (info_ptr != NULL) free(info_ptr);
                return;
            }

            Message* msg = new_message(*x_ptr, *y_ptr, *info_ptr);

            if (msg == NULL) {
                free(x_ptr);
                free(y_ptr);
                free(info_ptr);
                return;
            }

            free(x_ptr);
            free(y_ptr);
            free(info_ptr);

            // Send the message
            if (send_message(msg)) {
                // Message sent successfully
            } else {
                // Failed to send message
            }

            break;
        }

        // Set row back to low
        GPIO_setOutputLowOnPin(row_port[row], row_pins[row]);
    }

    // Restore all rows to high
    for (i = 0; i < NUM_ROWS; i++) {
        GPIO_setOutputHighOnPin(row_port[i], row_pins[i]);
    }

    // To enable interrupts for a specific pin
    GPIO_enableInterrupt(port, pin);
}
