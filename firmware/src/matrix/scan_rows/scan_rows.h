/**
 * @file scan_rows.h
 * @brief Matrix scanning function declarations
 * 
 * Functions for scanning keyboard matrix rows, managing the pressed keys
 * tracking list, and building HID report arrays with modifier keys.
 */

#ifndef SCAN_ROWS_H
#define SCAN_ROWS_H

    #include "pico/stdlib.h"
    #include <class/hid/hid.h>

    #include "src/usb/usb_descriptors/usb_descriptors.h"
    #include "src/usb/usb_callbacks/usb_callbacks.h"

    #include "../matrix.h"
    #include "../../global.h"
    #include "../keymap/keymap.h"

    uint8_t scan_rows(uint gpio);
    void keyboard_add_key(uint8_t row, uint8_t col);
    void keyboard_remove_key(uint8_t row, uint8_t col);

    void build_keycode_array(uint8_t* modifier, uint8_t* keycode);

#endif /* SCAN_ROWS_H */
