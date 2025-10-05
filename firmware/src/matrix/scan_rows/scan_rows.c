#include "scan_rows.h"

//--------------------------------------------------------------------+
// VARIABLES
//--------------------------------------------------------------------+
extern keyboard_state_t kbd_state;

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+
uint8_t scan_rows(uint gpio) {
    for (uint8_t r = 0; r < 5; r++) {
        // Set current row HIGH, all others LOW
        gpio_put(ROW_0, (r == 0) ? HIGH : LOW);
        gpio_put(ROW_1, (r == 1) ? HIGH : LOW);
        gpio_put(ROW_2, (r == 2) ? HIGH : LOW);
        gpio_put(ROW_3, (r == 3) ? HIGH : LOW);
        gpio_put(ROW_4, (r == 4) ? HIGH : LOW);
        
        busy_wait_us(10);
        
        // If column reads HIGH when this row is HIGH, the key is pressed here
        if (gpio_get(gpio) == HIGH) {
            return r;
        }
    }
    return 0xFF; // No row found
}

void build_keycode_array(uint8_t* modifier, uint8_t* keycode) {
    // Build keycode array from current pressed keys
    uint8_t key_idx = 0;
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count && key_idx < 6; i++) {
        uint8_t row = kbd_state.pressed_keys[i][0];
        uint8_t col = kbd_state.pressed_keys[i][1];
        
        // Skip the Fn key itself - non inviarlo come tasto
        if (row == FN_KEY_ROW && col == FN_KEY_COL) {
            continue;
        }
        
        // Passa il layer corrente alla funzione
        uint8_t hid_key = map_key_to_hid(row, col, kbd_state.current_layer);
        
        if (hid_key != 0) {
            // Check if it's a modifier key
            if (hid_key >= HID_KEY_CONTROL_LEFT && hid_key <= HID_KEY_GUI_RIGHT) {
                *modifier |= (1 << (hid_key - HID_KEY_CONTROL_LEFT));
            } else {
                // Regular key
                keycode[key_idx++] = hid_key;
            }
        }
    }
}

// Function to add a key press to the state (called from interrupt)
void keyboard_add_key(uint8_t row, uint8_t col) {
    // Check if key already in list
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
        if (kbd_state.pressed_keys[i][0] == row && 
            kbd_state.pressed_keys[i][1] == col) {
            return; // Already registered
        }
    }
    
    // Add new key if there's space
    if (kbd_state.pressed_keys_count < MAX_KEYS) {
        kbd_state.pressed_keys[kbd_state.pressed_keys_count][0] = row;
        kbd_state.pressed_keys[kbd_state.pressed_keys_count][1] = col;
        kbd_state.pressed_keys_count++;
        kbd_state.has_new_key = true;
    }
}

// Function to remove a key release from the state (called from interrupt)
void keyboard_remove_key(uint8_t row, uint8_t col) {
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
        if (kbd_state.pressed_keys[i][0] == row && 
            kbd_state.pressed_keys[i][1] == col) {
            // Remove by shifting remaining keys
            for (uint8_t j = i; j < kbd_state.pressed_keys_count - 1; j++) {
                kbd_state.pressed_keys[j][0] = kbd_state.pressed_keys[j + 1][0];
                kbd_state.pressed_keys[j][1] = kbd_state.pressed_keys[j + 1][1];
            }
            kbd_state.pressed_keys_count--;
            kbd_state.has_new_key = true;
            break;
        }
    }
}