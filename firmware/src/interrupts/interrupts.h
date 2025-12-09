/**
 * @file interrupts.h
 * @brief GPIO interrupt handler declarations
 * 
 * Callback function prototypes for handling keyboard matrix scanning,
 * rotary encoder rotation detection, and button press interrupts.
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "pico/stdlib.h"
    
    #include "../init/init.h"
    #include "../matrix/scan_rows/scan_rows.h"
    #include "../global.h"
    #include "../rotary_encoder/rotary_encoder.h"

    #define MATRIX_DEBOUNCE_TIME 5000
    #define ENCODER_CLK_DEBOUNCE_TIME 500
    #define ENCODER_BTN_DEBOUNCE_TIME 5000

    void gpio_callback(uint gpio, uint32_t events);

#endif /* INTERRUPTS_H */