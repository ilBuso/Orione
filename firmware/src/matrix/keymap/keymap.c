/**
 * @file keymap.c
 * @brief Keyboard layout implementation
 * 
 * Provides translation function from row/column matrix coordinates to
 * HID keycodes based on the currently active layer (base or Fn).
 */

#include "keymap.h"

//--------------------------------------------------------------------+

/**
 * @brief Map physical key position to HID keycode
 * 
 * Translates a physical key location (row, column) to its corresponding
 * HID keycode based on the active layer. Returns 0 for invalid positions
 * or unmapped keys.
 * 
 * @param row Row number (0-4)
 * @param col Column number (0-13)
 * @param layer Active layer (0 = base, 1 = Fn)
 * @return HID keycode value, or 0 if invalid/unmapped
 */
uint8_t map_key_to_hid(uint8_t row, uint8_t col, uint8_t layer) {    
    // validate row and column
    if (row >= 5 && col >= 14) {
        return 0;
    }

    // return keycode from right layer
    if (layer == 0) {
        return base_keymap[row][col];
    } else if (layer == 1) {
        return fn_keymap[row][col];
    }

    // invalid layer
    return 0;
}   