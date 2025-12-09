/**
 * @file scan_rows.c
 * @brief Matrix scanning implementation
 * 
 * Scans keyboard matrix to identify active keys, maintains a list of
 * currently pressed keys, and constructs HID keycode arrays including
 * modifier keys. Implements Fn key layer switching logic.
 */

#include "scan_rows.h"

//--------------------------------------------------------------------+

extern keyboard_state_t kbd_state;

//--------------------------------------------------------------------+

/**
 * @brief Scan matrix rows to identify which row a key is on
 * 
 * Tests each row by setting it HIGH while others are LOW, checking if
 * the specified column pin reads HIGH to determine the active row.
 * 
 * @param gpio Column GPIO pin number to test
 * @return Row number (0-4) if found, or 0xFF if no active row detected
 */
uint8_t scan_rows(uint gpio) {
    // test each row sequentially
    for (uint8_t r = 0; r < 5; r++) {
        // set current row HIGH, all others LOW
        gpio_put(ROW_0, (r == 0) ? HIGH : LOW);
        gpio_put(ROW_1, (r == 1) ? HIGH : LOW);
        gpio_put(ROW_2, (r == 2) ? HIGH : LOW);
        gpio_put(ROW_3, (r == 3) ? HIGH : LOW);
        gpio_put(ROW_4, (r == 4) ? HIGH : LOW);
        
        // allow signal to settle
        busy_wait_us(10);
        
        // if column reads HIGH when this row is HIGH, the key is pressed here
        if (gpio_get(gpio) == HIGH) {
            return r;
        }
    }
    return 0xFF; // no row found
}

// Helper function to check if a key is a consumer control key
bool is_consumer_key(uint16_t key) {
    // Media control keys
    if (key == HID_USAGE_CONSUMER_PLAY_PAUSE ||
        key == HID_USAGE_CONSUMER_SCAN_NEXT ||
        key == HID_USAGE_CONSUMER_SCAN_PREVIOUS)
        return true;
    
    // Brightness control keys
    if (key == HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT ||
        key == HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT)
        return true;
    
    // Volume keys (if you want them too)
    if (key == HID_USAGE_CONSUMER_MUTE ||
        key == HID_USAGE_CONSUMER_VOLUME_INCREMENT ||
        key == HID_USAGE_CONSUMER_VOLUME_DECREMENT)
        return true;
    
    return false;
}

/**
 * @brief Build HID keycode array from pressed keys
 * 
 * Constructs the HID report by iterating through currently pressed keys,
 * translating them to HID keycodes based on the active layer, and separating
 * modifier keys from regular keys. Skips the Fn key itself.
 * 
 * @param modifier Pointer to modifier byte (bitfield of active modifiers)
 * @param keycode Pointer to 6-byte array for regular keycodes
 */
void build_keycode_array(uint8_t* modifier, uint8_t* keycode) {
    static uint16_t last_consumer_code = 0;
    
    uint8_t key_idx = 0;
    uint16_t active_consumer_code = 0;
    
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count && key_idx < 6; i++) {
        uint8_t row = kbd_state.pressed_keys[i][0];
        uint8_t col = kbd_state.pressed_keys[i][1];
        
        // skip the Fn key itself
        if (row == FN_KEY_ROW && col == FN_KEY_COL) {
            continue;
        }
        
        // get HID keycode based on current layer
        uint16_t hid_key = map_key_to_hid(row, col, kbd_state.current_layer);

        if (hid_key != 0) {
            // check if it's a modifier key
            if (hid_key >= HID_KEY_CONTROL_LEFT && hid_key <= HID_KEY_GUI_RIGHT) {
                // set appropriate bit in modifier byte
                *modifier |= (1 << ((uint8_t)hid_key - HID_KEY_CONTROL_LEFT));
            }
            // check if it's a consumer control key
            else if (is_consumer_key(hid_key)) {
                active_consumer_code = hid_key;
            } 
            else {
                // regular key
                keycode[key_idx++] = (uint8_t)hid_key;
            }
        }
    }
    
    // Send consumer control report only when code changes
    if (active_consumer_code != last_consumer_code) {
        send_hid_report(REPORT_ID_CONSUMER_CONTROL, active_consumer_code);
        last_consumer_code = active_consumer_code;
    }
}

/**
 * @brief Add a key press to the tracked state
 * 
 * Registers a new key press by adding it to the pressed keys list.
 * Checks for duplicates to avoid registering the same key twice.
 * Called from interrupt context.
 * 
 * @param row Row number of the pressed key
 * @param col Column number of the pressed key
 */
void keyboard_add_key(uint8_t row, uint8_t col) {
    // check if key already in list
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
        if (kbd_state.pressed_keys[i][0] == row && 
            kbd_state.pressed_keys[i][1] == col) {
            return; // already registered
        }
    }
    
    // add new key if there's space
    if (kbd_state.pressed_keys_count < MAX_KEYS) {
        kbd_state.pressed_keys[kbd_state.pressed_keys_count][0] = row;
        kbd_state.pressed_keys[kbd_state.pressed_keys_count][1] = col;
        kbd_state.pressed_keys_count++;
        kbd_state.has_new_key = true;
    }
}

/**
 * @brief Remove a key release from the tracked state
 * 
 * Unregisters a key release by removing it from the pressed keys list
 * and shifting remaining keys down to fill the gap.
 * Called from interrupt context.
 * 
 * @param row Row number of the released key
 * @param col Column number of the released key
 */
void keyboard_remove_key(uint8_t row, uint8_t col) {
    for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
        // find the key in the pressed list
        if (kbd_state.pressed_keys[i][0] == row && 
            kbd_state.pressed_keys[i][1] == col) {
            // remove by shifting remaining keys
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