#pragma once

#include <stdbool.h>
#include <stdint.h>

#define NUM_ROWS 6
#define NUM_COLS 17

/**
 *
 * @param info
 * @return true if pressed, false if released
 */
static bool check_pressed_flag(uint8_t info) {
    return (info & 0x01) != 0;
}

/**
 *
 * @param info
 * @return true if rotary encoder key, false if normal keyboard key
 */
inline bool check_encoder_flag(uint8_t info) {
    return (info & 0x02) != 0;
}