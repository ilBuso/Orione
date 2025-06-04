//
// Created by Giulio Pimenoff Verdolin on 09/05/25.
//

#pragma once

#include <string.h>
#include <stddef.h>
#include "keycodes.h"

// X-Macro definition of all key codes and their string representations
#define KEYCODE_MAP(X) \
    /* Alphanumeric keys */ \
    X(KEY_A) X(KEY_B) X(KEY_C) X(KEY_D) X(KEY_E) X(KEY_F) X(KEY_G) X(KEY_H) X(KEY_I) \
    X(KEY_J) X(KEY_K) X(KEY_L) X(KEY_M) X(KEY_N) X(KEY_O) X(KEY_P) X(KEY_Q) X(KEY_R) \
    X(KEY_S) X(KEY_T) X(KEY_U) X(KEY_V) X(KEY_W) X(KEY_X) X(KEY_Y) X(KEY_Z) \
    /* Numbers */ \
    X(KEY_0) X(KEY_1) X(KEY_2) X(KEY_3) X(KEY_4) X(KEY_5) X(KEY_6) X(KEY_7) X(KEY_8) X(KEY_9) \
    /* Function keys */ \
    X(KEY_F1) X(KEY_F2) X(KEY_F3) X(KEY_F4) X(KEY_F5) X(KEY_F6) X(KEY_F7) X(KEY_F8) \
    X(KEY_F9) X(KEY_F10) X(KEY_F11) X(KEY_F12) \
    /* Modifier keys */ \
    X(KEY_SHIFT_LEFT) X(KEY_SHIFT_RIGHT) X(KEY_CONTROL_LEFT) X(KEY_CONTROL_RIGHT) \
    X(KEY_ALT_LEFT) X(KEY_ALT_RIGHT) X(KEY_SUPER_LEFT) X(KEY_SUPER_RIGHT) \
    /* Navigation keys */ \
    X(KEY_ESCAPE) X(KEY_TAB) X(KEY_CAPS_LOCK) X(KEY_ENTER) X(KEY_BACKSPACE) X(KEY_INSERT) \
    X(KEY_DELETE) X(KEY_HOME) X(KEY_END) X(KEY_PAGE_UP) X(KEY_PAGE_DOWN) \
    X(KEY_UP) X(KEY_DOWN) X(KEY_LEFT) X(KEY_RIGHT) \
    /* Special character keys */ \
    X(KEY_SPACE) X(KEY_GRAVE_ACCENT) X(KEY_MINUS) X(KEY_EQUAL) X(KEY_LEFT_BRACKET) \
    X(KEY_RIGHT_BRACKET) X(KEY_BACKSLASH) X(KEY_SEMICOLON) X(KEY_APOSTROPHE) \
    X(KEY_COMMA) X(KEY_PERIOD) X(KEY_SLASH) \
    /* Numpad keys */ \
    X(KEY_NUMPAD_0) X(KEY_NUMPAD_1) X(KEY_NUMPAD_2) X(KEY_NUMPAD_3) X(KEY_NUMPAD_4) \
    X(KEY_NUMPAD_5) X(KEY_NUMPAD_6) X(KEY_NUMPAD_7) X(KEY_NUMPAD_8) X(KEY_NUMPAD_9) \
    X(KEY_NUMPAD_DECIMAL) X(KEY_NUMPAD_DIVIDE) X(KEY_NUMPAD_MULTIPLY) X(KEY_NUMPAD_SUBTRACT) \
    X(KEY_NUMPAD_ADD) X(KEY_NUMPAD_ENTER) X(KEY_NUMPAD_EQUAL) \
    /* Lock keys */ \
    X(KEY_NUM_LOCK) X(KEY_SCROLL_LOCK) X(KEY_PRINT_SCREEN) X(KEY_PAUSE) \
    /* Media keys */ \
    X(KEY_MUTE) X(KEY_VOLUME_UP) X(KEY_VOLUME_DOWN) X(KEY_MEDIA_PLAY_PAUSE) \
    X(KEY_MEDIA_STOP) X(KEY_MEDIA_NEXT) X(KEY_MEDIA_PREVIOUS) \
    /* Miscellaneous */ \
    X(KEY_MENU) \
    /* Count (Not included in string conversion) */ \
    /* X(KEY_COUNT) */

// Convert from CrossPlatformKeyCode to string
const char* keycode_to_str(enum CrossPlatformKeyCode key_code) {
    switch (key_code) {
    #define X(key) case key: return #key;
    KEYCODE_MAP(X)
    #undef X
    default: return "UNKNOWN_KEY";
    }
}

// Convert from string to CrossPlatformKeyCode
enum CrossPlatformKeyCode str_to_keycode(const char* str) {
    #define X(key) if (strcmp(str, #key) == 0) return key;
    KEYCODE_MAP(X)
    #undef X

    // If no match found, could return a default or error value
    // Here returning KEY_COUNT to indicate "not found"
    return KEY_COUNT;
}

// Optional: Check if a string is a valid key code
int is_valid_keycode_str(const char* keyString) {
    #define X(key) if (strcmp(keyString, #key) == 0) return 1;
    KEYCODE_MAP(X)
    #undef X
    return 0;
}