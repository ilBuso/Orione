#ifndef SCAN_ROWS_H
#define SCAN_ROWS_H

    #include "pico/stdlib.h"
    #include "../matrix.h"
    #include "../../global.h"
    #include "../keymap/keymap.h"
    #include <class/hid/hid.h>

    uint8_t scan_rows(uint gpio);
    void keyboard_add_key(uint8_t row, uint8_t col);
    void keyboard_remove_key(uint8_t row, uint8_t col);

    void build_keycode_array(uint8_t* modifier, uint8_t* keycode);

#endif /* SCAN_ROWS_H */
