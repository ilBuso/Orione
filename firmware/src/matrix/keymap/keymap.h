/**
 * @file keymap.h
 * @brief Keyboard layout definitions
 * 
 * Defines key mappings for base and Fn layers, translating physical
 * matrix positions (row, column) to HID keycodes. Includes Fn key location.
 */

#ifndef KEYMAP_H
#define KEYMAP_H

    #include <class/hid/hid.h>

    static const uint8_t base_keymap[5][14] = {
        // Row 0
        {HID_KEY_GRAVE, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0, HID_KEY_MINUS, HID_KEY_EQUAL, HID_KEY_BACKSPACE},
        // Row 1
        {HID_KEY_TAB, HID_KEY_Q, HID_KEY_W, HID_KEY_E, HID_KEY_R, HID_KEY_T, HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BRACKET_LEFT, HID_KEY_BRACKET_RIGHT, HID_KEY_BACKSLASH},
        // Row 2
        {HID_KEY_CAPS_LOCK, HID_KEY_A, HID_KEY_S, HID_KEY_D, HID_KEY_F, HID_KEY_G, HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE, 0, HID_KEY_ENTER},
        // Row 3
        {HID_KEY_SHIFT_LEFT, HID_KEY_Z, HID_KEY_X, HID_KEY_C, HID_KEY_V, HID_KEY_B, HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH, 0, 0, HID_KEY_SHIFT_RIGHT},
        // Row 4
        {HID_KEY_CONTROL_LEFT, HID_KEY_GUI_LEFT, HID_KEY_ALT_LEFT, 0, 0, HID_KEY_SPACE, 0, 0, 0, 0, HID_KEY_ARROW_LEFT, HID_KEY_ARROW_UP, HID_KEY_ARROW_DOWN, HID_KEY_ARROW_RIGHT}
    };

    static const uint8_t fn_keymap[5][14] = {
        // Row 0
        {HID_KEY_ESCAPE, HID_KEY_F1, HID_KEY_F2, HID_KEY_F3, HID_KEY_F4, HID_KEY_F5, HID_KEY_F6, HID_KEY_F7, HID_KEY_F8, HID_KEY_F9, HID_KEY_F10, HID_KEY_F11, HID_KEY_F12, 0},
        // Row 1
        {0, 0, 0, 0, 0, 0, 0, 0, 0, HID_USAGE_CONSUMER_BRIGHTNESS_DECREMENT, HID_USAGE_CONSUMER_BRIGHTNESS_INCREMENT, 0, 0, 0},
        // Row 2
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        // Row 3
        {0, 0, 0, 0, 0, 0, 0, 0, 0, HID_KEY_ALT_RIGHT, 0, 0, 0, 0},
        // Row 4
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, HID_USAGE_CONSUMER_SCAN_PREVIOUS, HID_USAGE_CONSUMER_PLAY_PAUSE, HID_USAGE_CONSUMER_PLAY_PAUSE, HID_USAGE_CONSUMER_SCAN_NEXT}
    };
    uint8_t map_key_to_hid(uint8_t row, uint8_t col, uint8_t layer);

#endif /* KEYMAP_H */