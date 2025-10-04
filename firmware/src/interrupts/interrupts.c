#include "interrupts.h"

//--------------------------------------------------------------------+
// EXTERNAL FUNCTIONS
//--------------------------------------------------------------------+
extern void keyboard_add_key(uint8_t row, uint8_t col);
extern void keyboard_remove_key(uint8_t row, uint8_t col);

//--------------------------------------------------------------------+
// VARIABLES
//--------------------------------------------------------------------+
static uint32_t last_interrupt_time[14] = {0}; // One per column
extern keyboard_state_t kbd_state;

extern uint32_t last_encoder_time;
extern uint32_t last_button_time;
extern uint8_t last_clk_state;
extern rotary_encoder_state_t rotary_state;

//--------------------------------------------------------------------+
// SUPPORT FUNCTIONS
//--------------------------------------------------------------------+
void keyboard_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    uint8_t column = gpio - COLUMN_0;
    
    // Debounce check
    if (column < 14) {
        if (current_time - last_interrupt_time[column] < DEBOUNCE_TIME_US) {
            return;
        }
        last_interrupt_time[column] = current_time;
    }
    
    if (column >= 14) return;
    
    bool is_press = (events & GPIO_IRQ_EDGE_RISE);
    
    if (is_press) {
        // Key press - scan to find row
        uint8_t row = 0xFF;
        
        for (uint8_t r = 0; r < 5; r++) {
            // Set current row HIGH, all others LOW
            gpio_put(ROW_0, (r == 0) ? HIGH : LOW);
            gpio_put(ROW_1, (r == 1) ? HIGH : LOW);
            gpio_put(ROW_2, (r == 2) ? HIGH : LOW);
            gpio_put(ROW_3, (r == 3) ? HIGH : LOW);
            gpio_put(ROW_4, (r == 4) ? HIGH : LOW);
            
            busy_wait_us(10);
            
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
            keyboard_add_key(row, column);
        }
    } else {
        // Key release - we need to find which key in this column was pressed
        // We can't scan for it, so we search our pressed keys list
        for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
            if (kbd_state.pressed_keys[i][1] == column) {
                // Found a key in this column, remove it
                uint8_t row = kbd_state.pressed_keys[i][0];
                keyboard_remove_key(row, column);
                break; // Assume only one key per column (single key press)
            }
        }
    }
}

// Rotary encoder interrupt callback
void rotary_clk_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    
    // Debounce
    if (current_time - last_encoder_time < ENCODER_DEBOUNCE_US) {
        return;
    }
    last_encoder_time = current_time;
    
    // Read current state
    uint8_t clk_state = gpio_get(ROTARY_CLK);
    uint8_t dt_state = gpio_get(ROTARY_DT);
    
    // Detect direction on falling edge of CLK
    if (last_clk_state == 1 && clk_state == 0) {
        if (dt_state == 1) {
            // Clockwise
            rotary_state.direction = 1;
        } else {
            // Counter-clockwise
            rotary_state.direction = -1;
        }
        rotary_state.has_event = true;
    }
    
    last_clk_state = clk_state;
}

// Button interrupt callback
void rotary_button_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    
    // Debounce
    if (current_time - last_button_time < BUTTON_DEBOUNCE_US) {
        return;
    }
    last_button_time = current_time;
    
    // Button pressed (falling edge, assuming active low with pull-up)
    if (events & GPIO_IRQ_EDGE_FALL) {
        rotary_state.button_pressed = true;
        rotary_state.has_event = true;
    }
}

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+
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
            keyboard_callback(gpio, events);
            return;
        }
        break;
    }
}