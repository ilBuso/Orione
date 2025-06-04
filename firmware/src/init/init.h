/**
 * @file init.h
 * @brief Initialization functions for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file declares functions for initializing the GPIO pins, UART,
 * and other peripherals needed for keyboard matrix scanning.
 */

#ifndef INIT_H
#define INIT_H

    // Includes
    // TI
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include <ti/devices/msp432p4xx/driverlib/Timer32.h>
    #include "msp.h"

    /**
    * @brief Initialize GPIO pins for keyboard matrix
    * 
    * Configures the GPIO pins used for keyboard matrix scanning:
    * - Sets row pins as outputs (initially high)
    * - Sets column pins as inputs with pull-down resistors
    */
    void GPIO_init(void);

    /**
    * @brief Initialize GPIO interrupts for keyboard scanning
    * 
    * Configures interrupt settings for all column pins including:
    * - Setting edge detection for LOW to HIGH transitions
    * - Clearing any pending interrupts
    * - Enabling interrupts for relevant pins
    * - Enabling interrupt handling in the NVIC
    */
    void interrupt_init(void);

    /**
    * @brief Initialize UART communication
    * 
    * Configures the UART module for transmitting key codes:
    * - Sets up UART pins as peripheral function
    * - Configures UART parameters (baud rate, data bits, etc.)
    * - Sets up the system clock for proper UART operation
    * - Configures LED pin for visual feedback
    */
    void UART_init(void);

    /**
    * @brief Main initialization function
    * 
    * Calls all required initialization functions in the proper order:
    * - GPIO_init(): Configure GPIO pins
    * - interrupt_init(): Configure interrupts
    * - UART_init(): Configure UART
    */
    void init(void);

#endif // INIT_H
