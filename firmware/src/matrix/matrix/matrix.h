/**
 * @file matrix.h
 * @brief Functions for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file contains the definition for the helper functions related to
 * the keyboard matrix configuration.
 */

#ifndef MATRIX_H
#define MATRIX_H

    // Includes
    // TI
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    // Libraries
    #include <stdint.h>

    // Header files
    #include "../global/global.h"

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

#endif // MATRIX_H
 