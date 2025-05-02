/**
 * @file keyboard_globals.h
 * @brief Global definitions and functions for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file contains the global constants, arrays, and helper functions
 * related to the keyboard matrix configuration. It defines the mapping
 * between physical pins and logical rows/columns, as well as the keycode
 * mapping for the keyboard matrix.
 */

#ifndef GLOBAL_H
#define GLOBAL_H

    // Includes
    // TI
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    // Libraries
    #include <stdint.h>

    // Defines
    /** @brief Number of rows in the keyboard matrix */
    #define NUM_ROWS 6
    /** @brief Number of columns in the keyboard matrix */
    #define NUM_COLS 17

    /** @brief Number of pins paired with Port 2 in the keyboard matrix */
    #define PORT_2_NUM_PINS 4
    /** @brief Number of pins paired with Port 3 in the keyboard matrix */
    #define PORT_3_NUM_PINS 1
    /** @brief Number of pins paired with Port 4 in the keyboard matrix */
    #define PORT_4_NUM_PINS 5
    /** @brief Number of pins paired with Port 5 in the keyboard matrix */
    #define PORT_5_NUM_PINS 4
    /** @brief Number of pins paired with Port 6 in the keyboard matrix */
    #define PORT_6_NUM_PINS 3

    /** @brief Volume variable */
    volatile uint8_t volume;

    /** @brief Last read state of Channel A for rotary encoder */
    extern volatile uint8_t last_channel_a_state;
    
    /** @brief Previous sent volume level */
    extern volatile uint8_t prev_volume;

    /** @brief Array of GPIO ports for each keyboard row*/
    extern const uint_fast16_t row_port[NUM_ROWS];
    /** @brief Array of GPIO pins for each keyboard row */
    extern const uint_fast16_t row_pins[NUM_ROWS];

    /**
    * @brief Column mapping for GPIO Port 2
    * 
    * Structure: {pin, column_index}
    */
    extern const uint8_t port_2_columns[PORT_2_NUM_PINS][2];
    /**
    * @brief Column mapping for GPIO Port 3
    * 
    * Structure: {pin, column_index}
    */
    extern const uint8_t port_3_columns[PORT_3_NUM_PINS][2];
    /**
    * @brief Column mapping for GPIO Port 4
    * 
    * Structure: {pin, column_index}
    */
    extern const uint8_t port_4_columns[PORT_4_NUM_PINS][2];
    /**
    * @brief Column mapping for GPIO Port 5
    * 
    * Structure: {pin, column_index}
    */
    extern const uint8_t port_5_columns[PORT_5_NUM_PINS][2];
    /**
    * @brief Column mapping for GPIO Port 6
    * 
    * Structure: {pin, column_index}
    */
    extern const uint8_t port_6_columns[PORT_6_NUM_PINS][2];

    /**
    * @brief Key code mapping for the keyboard matrix
    * 
    * 2D array that maps physical keyboard positions to logical key codes.
    * The first index is the row, the second index is the column.
    * A value of -1 indicates no key at that position.
    */
    extern const int8_t keys[NUM_ROWS][NUM_COLS];

    /**
    * @brief Translates a GPIO port and pin to a column index
    * 
    * @param port The GPIO port identifier (e.g., GPIO_PORT_P2)
    * @param pin The GPIO pin identifier (e.g., GPIO_PIN3)
    * @return int8_t The column index in the keyboard matrix, or -1 if not found
    */
    int8_t get_column_index(uint_fast16_t port, uint_fast16_t pin);

    /**
    * @brief Scans rows to determine which key was pressed
    * 
    * When a column interrupt occurs, this function scans all rows to find
    * which key in the matrix was pressed. When found, it sends the corresponding
    * key code over UART and provides visual feedback via the LED.
    * 
    * @param port The GPIO port that triggered the interrupt
    * @param pin The GPIO pin that triggered the interrupt
    */
    void scan_rows(uint_fast16_t port, uint_fast16_t pin);

#endif // GLOBAL_H
