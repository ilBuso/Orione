#include "keymap.h"

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+
uint8_t map_key_to_hid(uint8_t row, uint8_t col, uint8_t layer) {    
    if (row >= 5 && col >= 14) {
        return 0;
    } else {
        if (layer == 0) {
            return base_keymap[row][col];
        } else {
            return fn_keymap[row][col];
        }
    }
    return 0;
}