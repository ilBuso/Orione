/**
 * @file rotary_encoder.c
 * @brief Rotary encoder state management
 * 
 * Manages rotary encoder state including rotation direction and button
 * press status. Provides interrupt-safe atomic access for reading and
 * clearing encoder events from the main loop.
 */

#include "rotary_encoder.h"

//--------------------------------------------------------------------+

uint32_t last_encoder_time = 0;             ///< Timestamp of last encoder rotation event
uint32_t last_button_time = 0;              ///< Timestamp of last button press event
uint8_t last_clk_state = 1;                 ///< Previous state of CLK pin for edge detection
rotary_encoder_state_t rotary_state = {0};  ///< Current encoder state


//--------------------------------------------------------------------+

/**
 * @brief Get and clear current rotary encoder state
 * 
 * Atomically reads the current encoder state (rotation direction and button
 * press) and clears it for the next event. Uses interrupt disabling to ensure
 * thread-safe access between main loop and interrupt handlers.
 * 
 * @param direction Pointer to store rotation direction (-1=CCW, 0=none, 1=CW)
 * @param button_pressed Pointer to store button press status (true if pressed)
 */
void rotary_encoder_get_state(int8_t* direction, bool* button_pressed) {
    // quick check without disabling interrupts
    if (!rotary_state.has_event) {
        *direction = 0;
        *button_pressed = false;
        return;
    }
    
    // disable interrupts briefly to read state atomically
    uint32_t status = save_and_disable_interrupts();
    
    // copy current state
    *direction = rotary_state.direction;
    *button_pressed = rotary_state.button_pressed;
    
    // Clear state
    rotary_state.direction = 0;
    rotary_state.button_pressed = false;
    rotary_state.has_event = false;
    
    // restore interrupts
    restore_interrupts(status);
}