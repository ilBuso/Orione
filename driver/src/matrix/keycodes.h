//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#pragma once

enum CrossPlatformKeyCode {
    // Alphanumeric keys
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    // Numbers
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    // Function keys
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    // Modifier keys
    KEY_SHIFT_LEFT,
    KEY_SHIFT_RIGHT,
    KEY_CONTROL_LEFT,
    KEY_CONTROL_RIGHT,
    KEY_ALT_LEFT,
    KEY_ALT_RIGHT,
    KEY_SUPER_LEFT,      // Windows/Command key
    KEY_SUPER_RIGHT,     // Windows/Command key

    // Navigation keys
    KEY_ESCAPE,
    KEY_TAB,
    KEY_CAPS_LOCK,
    KEY_ENTER,
    KEY_BACKSPACE,
    KEY_INSERT,
    KEY_DELETE,
    KEY_HOME,
    KEY_END,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,

    // Special character keys
    KEY_SPACE,
    KEY_GRAVE_ACCENT,    // `
    KEY_MINUS,           // -
    KEY_EQUAL,           // =
    KEY_LEFT_BRACKET,    // [
    KEY_RIGHT_BRACKET,   // ]
    KEY_BACKSLASH,       //
    KEY_SEMICOLON,       // ;
    KEY_APOSTROPHE,      // '
    KEY_COMMA,           // ,
    KEY_PERIOD,          // .
    KEY_SLASH,           // /

    // Numpad keys
    KEY_NUMPAD_0,
    KEY_NUMPAD_1,
    KEY_NUMPAD_2,
    KEY_NUMPAD_3,
    KEY_NUMPAD_4,
    KEY_NUMPAD_5,
    KEY_NUMPAD_6,
    KEY_NUMPAD_7,
    KEY_NUMPAD_8,
    KEY_NUMPAD_9,
    KEY_NUMPAD_DECIMAL,
    KEY_NUMPAD_DIVIDE,
    KEY_NUMPAD_MULTIPLY,
    KEY_NUMPAD_SUBTRACT,
    KEY_NUMPAD_ADD,
    KEY_NUMPAD_ENTER,
    KEY_NUMPAD_EQUAL,

    // Lock keys
    KEY_NUM_LOCK,
    KEY_SCROLL_LOCK,
    KEY_PRINT_SCREEN,
    KEY_PAUSE,

    // Media keys (commonly available on first layer)
    KEY_MUTE,
    KEY_VOLUME_UP,
    KEY_VOLUME_DOWN,
    KEY_MEDIA_PLAY_PAUSE,
    KEY_MEDIA_STOP,
    KEY_MEDIA_NEXT,
    KEY_MEDIA_PREVIOUS,

    // Miscellaneous
    KEY_MENU,            // Application/Context menu key

    // Keep track of total keys
    KEY_COUNT
};