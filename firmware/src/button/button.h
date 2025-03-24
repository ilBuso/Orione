/**
 * @file button.h
 * @brief Implementation of the button event handlers.
 * @author Michele Pezzo
 * @date   March 2025
 *
 * This file implements the event handler for button events.
 * todo: we could save the state of the button in a global variable
 */

#ifndef BUTTON_H
#define BUTTON_H

    // Includes
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    // Libraries
    #include <stdint.h>

    // Functions
    void on_button_press(uint_fast16_t port, uint_fast16_t pin);
    void on_button_release(uint_fast16_t port, uint_fast16_t pin);
#endif // BUTTON_H
