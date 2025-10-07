/**
 * @file interrupts.c
 * @brief GPIO interrupt handler implementation
 * 
 * Implements interrupt service routines with debouncing for keyboard matrix
 * key detection, rotary encoder direction sensing, and Fn layer switching.
 * Handles both key press and release events with proper state management.
 */

#include "interrupts.h"

//--------------------------------------------------------------------+

static uint32_t last_interrupt_time[14] = {0};
extern keyboard_state_t kbd_state;

extern uint32_t last_encoder_time;
extern uint32_t last_button_time;
extern uint8_t last_clk_state;
extern rotary_encoder_state_t rotary_state;

extern void keyboard_add_key(uint8_t row, uint8_t col);
extern void keyboard_remove_key(uint8_t row, uint8_t col);

//--------------------------------------------------------------------+

/**
 * @brief Keyboard matrix interrupt callback
 * 
 * Handles GPIO interrupts from column pins to detect key presses and releases.
 * Implements debouncing, row scanning to identify which key was pressed,
 * and special handling for the Fn layer key.
 * 
 * @param gpio GPIO pin number that triggered the interrupt
 * @param events Interrupt event flags (EDGE_RISE or EDGE_FALL)
 */
void keyboard_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    uint8_t column = gpio - COLUMN_0;
    
    // Debounce check
    if (column < 14) {
        if (current_time - last_interrupt_time[column] < MATRIX_DEBOUNCE_TIME) {
            return;
        }
        last_interrupt_time[column] = current_time;
    }
    
    // validate the column number
    if (column >= 14) return;
    
    // determine if this is a press (rising edge) or release (falling edge)
    bool is_press = (events & GPIO_IRQ_EDGE_RISE);
    
    if (is_press) {
        uint8_t row = 0xFF;

        // iterate on every row to find the right one
        for (uint8_t r = 0; r < 5; r++) {
            gpio_put(ROW_0, (r == 0) ? HIGH : LOW);
            gpio_put(ROW_1, (r == 1) ? HIGH : LOW);
            gpio_put(ROW_2, (r == 2) ? HIGH : LOW);
            gpio_put(ROW_3, (r == 3) ? HIGH : LOW);
            gpio_put(ROW_4, (r == 4) ? HIGH : LOW);
            
            // Wait for signal to stabilize
            busy_wait_us(10);
            
            // if column reads HIGH, this is the active row
            if (gpio_get(gpio) == HIGH) {
                row = r;
                break;
            }
        }
        
        // Restore all rows to HIGH
        gpio_put(ROW_0, HIGH);
        gpio_put(ROW_1, HIGH);
        gpio_put(ROW_2, HIGH);
        gpio_put(ROW_3, HIGH);
        gpio_put(ROW_4, HIGH);
        
        if (row != 0xFF) {
            // check if this is the Fn key
            if (row == FN_KEY_ROW && column == FN_KEY_COL) {
                // activate fn layer
                kbd_state.current_layer = 1;
            } else {
                // normal key
                keyboard_add_key(row, column);
            }
        }
    } else {
        // key release check
        bool fn_key_released = false;
        
        // find which row this column corresponds to by searching pressed keys
        uint8_t row = 0xFF;
        for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
            if (kbd_state.pressed_keys[i][1] == column) {
                row = kbd_state.pressed_keys[i][0];
                break;
            }
        }
        
        // if not found in pressed_keys and layer is 1, assume it's Fn being released
        if (row == 0xFF && kbd_state.current_layer == 1) {
            // Assume it's the Fn key being released
            kbd_state.current_layer = 0;
            fn_key_released = true;
        }
        
        // explicit check if this is the Fn key position
        if (row == FN_KEY_ROW && column == FN_KEY_COL) {
            kbd_state.current_layer = 0;  // Back to base layer
            fn_key_released = true;
        }
        
        // remove the key from pressed list
        if (!fn_key_released && row != 0xFF) {
            keyboard_remove_key(row, column);
        }
    }
}

/**
 * @brief Rotary encoder CLK pin interrupt callback
 * 
 * Detects rotation direction by reading CLK and DT pin states.
 * Uses falling edge detection with debouncing to determine clockwise
 * or counter-clockwise rotation.
 * 
 * @param gpio GPIO pin number (should be ROTARY_CLK)
 * @param events Interrupt event flags
 */
void rotary_clk_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    
    // Debounce
    if (current_time - last_encoder_time < ENCODER_CLK_DEBOUNCE_TIME) {
        return;
    }
    last_encoder_time = current_time;

    // Wait for signal to stabilize
    busy_wait_us(100); 
    
    // Read current state
    uint8_t clk_state = gpio_get(ROTARY_CLK);
    uint8_t dt_state = gpio_get(ROTARY_DT);
    
    // Detect direction on falling edge of CLK
    if (last_clk_state == 1 && clk_state == 0) {
        if (dt_state == 1) {
            // DT is HIGH during CLK fall = Clockwise rotation
            rotary_state.direction = 1;
        } else {
            // DT is LOW during CLK fall = Counter-clockwise rotation
            rotary_state.direction = -1;
        }
        rotary_state.has_event = true;
    }
    
    // store current state
    last_clk_state = clk_state;
}

/**
 * @brief Rotary encoder button interrupt callback
 * 
 * Handles button press detection on the rotary encoder's integrated switch.
 * Uses falling edge detection (button is active LOW) with debouncing.
 * 
 * @param gpio GPIO pin number (should be ROTARY_SW)
 * @param events Interrupt event flags
 */
void rotary_button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    
    // Debounce
    if (current_time - last_button_time < ENCODER_BTN_DEBOUNCE_TIME) {
        return;
    }
    last_button_time = current_time;
    
    // Button pressed
    if (events & GPIO_IRQ_EDGE_FALL) {
        rotary_state.button_pressed = true;
        rotary_state.has_event = true;
    }
}

//--------------------------------------------------------------------+

/**
 * @brief Main GPIO interrupt dispatcher
 * 
 * Routes GPIO interrupts to the appropriate handler based on which pin
 * triggered the interrupt. This single callback handles all keyboard matrix
 * columns, rotary encoder CLK, and rotary encoder button.
 * 
 * @param gpio GPIO pin number that triggered the interrupt
 * @param events Interrupt event flags (EDGE_RISE, EDGE_FALL, etc.)
 */
void gpio_callback(uint gpio, uint32_t events) {
    switch (gpio) {
        case ROTARY_CLK: {
            rotary_clk_callback(gpio, events);
            return;
        }
        break;
        
        case ROTARY_SW: {
            rotary_button_callback(gpio, events);
            return;
        }
        break;

        default: {
            // all other GPIOs are keyboard matrix columns
            keyboard_callback(gpio, events);
            return;
        }
        break;
    }
}