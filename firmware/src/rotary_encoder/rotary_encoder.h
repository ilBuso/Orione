#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

    #include <stdint.h>
    #include <stdbool.h>

    #include "pico/stdlib.h"
    #include "hardware/sync.h"
    
    #define ROTARY_CLK 19    // CLK pin (A)
    #define ROTARY_DT 20     // DT pin (B)
    #define ROTARY_SW 21     // Switch pin (button)

    // Rotary encoder state
    typedef struct {
        volatile int8_t direction;     // -1 for CCW, 1 for CW, 0 for no change
        volatile bool button_pressed;   // true when button is pressed
        volatile bool has_event;        // true when there's a new event to process
    } rotary_encoder_state_t;

    extern uint32_t last_encoder_time;
    extern uint32_t last_button_time;
    extern uint8_t last_clk_state;
    extern rotary_encoder_state_t rotary_state;

    // Get current state (call from main loop)
    void rotary_encoder_get_state(int8_t* direction, bool* button_pressed);

#endif /* ROTARY_ENCODER_H */