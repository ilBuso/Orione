//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#pragma once
#include <stddef.h>
#include "matrix/keycodes.h"

// Assuming CrossPlatformKeyCode enum is defined elsewhere
// Creates a 6x17 matrix representing a standard US QWERTY keyboard layout
// Each row represents a row on the keyboard, starting from the top (function keys)
// 0 is used for empty/non-existent key positions

const enum CrossPlatformKeyCode keyboard_matrix[6][17] = {
    // Row 0: Function keys row
    {
        KEY_ESCAPE, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
        KEY_PRINT_SCREEN, KEY_SCROLL_LOCK, KEY_PAUSE, 0
    },

    // Row 1: Number row
    {
        KEY_GRAVE_ACCENT, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0, KEY_MINUS, KEY_EQUAL,
        KEY_BACKSPACE, KEY_INSERT, KEY_HOME, KEY_PAGE_UP
    },

    // Row 2: QWERTY row
    {
        KEY_TAB, KEY_Q, KEY_W, KEY_E, KEY_R, KEY_T, KEY_Y, KEY_U, KEY_I, KEY_O, KEY_P, KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET,
        KEY_BACKSLASH, KEY_DELETE, KEY_END, KEY_PAGE_DOWN
    },

    // Row 3: ASDF row
    {
        KEY_CAPS_LOCK, KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J, KEY_K, KEY_L, KEY_SEMICOLON, KEY_APOSTROPHE,
        KEY_ENTER, 0, 0, 0, 0
    },

    // Row 4: ZXCV row
    {
        KEY_SHIFT_LEFT, 0, KEY_Z, KEY_X, KEY_C, KEY_V, KEY_B, KEY_N, KEY_M, KEY_COMMA, KEY_PERIOD, KEY_SLASH,
        KEY_SHIFT_RIGHT, 0, KEY_UP, 0, 0
    },

    // Row 5: Bottom row
    {
        KEY_CONTROL_LEFT, KEY_SUPER_LEFT, KEY_ALT_LEFT, KEY_SPACE, KEY_ALT_RIGHT, KEY_SUPER_RIGHT, KEY_MENU,
        KEY_CONTROL_RIGHT, 0, 0, 0, 0, 0, KEY_LEFT, KEY_DOWN, KEY_RIGHT, 0
    }
};