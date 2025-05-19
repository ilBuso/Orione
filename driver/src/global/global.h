#pragma once

#include <stdint.h>

#define NUM_ROWS 6
#define NUM_COLS 17

inline bool check_pressed_flag(uint8_t info) {
    return (info & 0x01) != 0;
}

inline bool check_encoder_flag(uint8_t info) {
    return (info & 0x02) != 0;
}