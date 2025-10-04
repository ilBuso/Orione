#include "keymap.h"

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+
uint8_t map_key_to_hid(uint8_t row, uint8_t col) {    
    if (row < 5 && col < 14) {
        return keymap[row][col];
    }
    return 0;
}