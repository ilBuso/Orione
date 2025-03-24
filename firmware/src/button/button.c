/**
* @file button.c
 * @brief Implementation of the button event handlers.
 * @author Alessandro Busola
 * @date   March 2025
 *
 * This file implements the event handler for button events.
 * todo: we could save the state of the button in a global variable
 */

#include "button.h"

void on_button_press(uint_fast16_t port, uint_fast16_t pin) {
    // if the button is pressed, we set the state to 1
    GPIO_disableInterrupt(port, pin);
}

void on_button_release(uint_fast16_t port, uint_fast16_t pin) {
    // if the button is released, we set the state to 0
    GPIO_enableInterrupt(port, pin);
}