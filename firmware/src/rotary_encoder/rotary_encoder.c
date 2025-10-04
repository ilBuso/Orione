#include "rotary_encoder.h"

uint32_t last_encoder_time = 0;
uint32_t last_button_time = 0;
uint8_t last_clk_state = 1;
rotary_encoder_state_t rotary_state = {0};

void rotary_encoder_get_state(int8_t* direction, bool* button_pressed) {
    if (!rotary_state.has_event) {
        *direction = 0;
        *button_pressed = false;
        return;
    }
    
    // Disable interrupts briefly to read state atomically
    uint32_t status = save_and_disable_interrupts();
    
    *direction = rotary_state.direction;
    *button_pressed = rotary_state.button_pressed;
    
    // Clear state
    rotary_state.direction = 0;
    rotary_state.button_pressed = false;
    rotary_state.has_event = false;
    
    restore_interrupts(status);
}