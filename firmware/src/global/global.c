/**
 * @file global.c
 * @brief Implementation of global variables and helper functions
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file implements the global arrays and helper functions declared in
 * global.h for keyboard matrix scanning functionality.
 */

#include "global.h"

//Initialise volume at a default value
volatile uint8_t volume = 50;

// Mapping arrays for rows
const uint_fast16_t row_port[NUM_ROWS] = {
    GPIO_PORT_P5, GPIO_PORT_P1, GPIO_PORT_P1, GPIO_PORT_P5, GPIO_PORT_P5, GPIO_PORT_P3
};
const uint_fast16_t row_pins[NUM_ROWS] = {
    GPIO_PIN7, GPIO_PIN6, GPIO_PIN7, GPIO_PIN0, GPIO_PIN2, GPIO_PIN6
};

// Column to index mapping for each port
// Structure: {pin, column_index}
const uint8_t port_2_columns[PORT_2_NUM_PINS][2] = {
    {GPIO_PIN3, 10},
    {GPIO_PIN4, 14},
    {GPIO_PIN6, 15},
    {GPIO_PIN7, 16}
};

const uint8_t port_3_columns[PORT_3_NUM_PINS][2] = {
    {GPIO_PIN5, 8}
};

const uint8_t port_4_columns[PORT_4_NUM_PINS][2] = {
    {GPIO_PIN0, 6},
    {GPIO_PIN2, 5},
    {GPIO_PIN4, 4},
    {GPIO_PIN5, 3},
    {GPIO_PIN7, 2}
};

const uint8_t port_5_columns[PORT_5_NUM_PINS][2] = {
    {GPIO_PIN1, 9},
    {GPIO_PIN4, 1},
    {GPIO_PIN5, 0},
    {GPIO_PIN6, 13}
};

const uint8_t port_6_columns[PORT_6_NUM_PINS][2] = {
    {GPIO_PIN1, 7},
    {GPIO_PIN6, 12},
    {GPIO_PIN7, 11}
};

// Key-map
const int8_t keys[NUM_ROWS][NUM_COLS] = {
    {1, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88, 99, 70, 119, -1},
    {41, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, -1, -1},
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 43, -1, -1, -1},
    {58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 28, -1, -1, -1, -1},
    {42, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, -1, -1, 103, -1, -1},
    {29, 125, 56, 57, -1, -1, -1, 100, 126, 97, -1, 105, 108, 106, -1, -1, -1}
};

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
            int8_t key_code = keys[row][column];

            // Toggle LED for visual feedback
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            // Send key code
            UART_transmitData(EUSCI_A2_BASE, key_code);

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
