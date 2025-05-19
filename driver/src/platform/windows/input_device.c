//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#import "core/io/input_device.h"
#include "matrix/keycodes.h"

#include <windows.h>

WORD convertToWindowsKeyCode(const enum CrossPlatformKeyCode keyCode) {
    switch (keyCode) {
        // Alphabetic keys
        case KEY_A: return 'A';
        case KEY_B: return 'B';
        case KEY_C: return 'C';
        case KEY_D: return 'D';
        case KEY_E: return 'E';
        case KEY_F: return 'F';
        case KEY_G: return 'G';
        case KEY_H: return 'H';
        case KEY_I: return 'I';
        case KEY_J: return 'J';
        case KEY_K: return 'K';
        case KEY_L: return 'L';
        case KEY_M: return 'M';
        case KEY_N: return 'N';
        case KEY_O: return 'O';
        case KEY_P: return 'P';
        case KEY_Q: return 'Q';
        case KEY_R: return 'R';
        case KEY_S: return 'S';
        case KEY_T: return 'T';
        case KEY_U: return 'U';
        case KEY_V: return 'V';
        case KEY_W: return 'W';
        case KEY_X: return 'X';
        case KEY_Y: return 'Y';
        case KEY_Z: return 'Z';

        // Numeric keys
        case KEY_0: return '0';
        case KEY_1: return '1';
        case KEY_2: return '2';
        case KEY_3: return '3';
        case KEY_4: return '4';
        case KEY_5: return '5';
        case KEY_6: return '6';
        case KEY_7: return '7';
        case KEY_8: return '8';
        case KEY_9: return '9';

        // Function keys
        case KEY_F1: return VK_F1;
        case KEY_F2: return VK_F2;
        case KEY_F3: return VK_F3;
        case KEY_F4: return VK_F4;
        case KEY_F5: return VK_F5;
        case KEY_F6: return VK_F6;
        case KEY_F7: return VK_F7;
        case KEY_F8: return VK_F8;
        case KEY_F9: return VK_F9;
        case KEY_F10: return VK_F10;
        case KEY_F11: return VK_F11;
        case KEY_F12: return VK_F12;

        // Modifier keys
        case KEY_SHIFT_LEFT: return VK_LSHIFT;
        case KEY_SHIFT_RIGHT: return VK_RSHIFT;
        case KEY_CONTROL_LEFT: return VK_LCONTROL;
        case KEY_CONTROL_RIGHT: return VK_RCONTROL;
        case KEY_ALT_LEFT: return VK_LMENU;
        case KEY_ALT_RIGHT: return VK_RMENU;
        case KEY_SUPER_LEFT: return VK_LWIN;
        case KEY_SUPER_RIGHT: return VK_RWIN;

        // Navigation keys
        case KEY_ESCAPE: return VK_ESCAPE;
        case KEY_TAB: return VK_TAB;
        case KEY_CAPS_LOCK: return VK_CAPITAL;
        case KEY_ENTER: return VK_RETURN;
        case KEY_BACKSPACE: return VK_BACK;
        case KEY_INSERT: return VK_INSERT;
        case KEY_DELETE: return VK_DELETE;
        case KEY_HOME: return VK_HOME;
        case KEY_END: return VK_END;
        case KEY_PAGE_UP: return VK_PRIOR;
        case KEY_PAGE_DOWN: return VK_NEXT;
        case KEY_UP: return VK_UP;
        case KEY_DOWN: return VK_DOWN;
        case KEY_LEFT: return VK_LEFT;
        case KEY_RIGHT: return VK_RIGHT;

        // Special character keys
        case KEY_SPACE: return VK_SPACE;
        case KEY_GRAVE_ACCENT: return VK_OEM_3;  // `
        case KEY_MINUS: return VK_OEM_MINUS;     // -
        case KEY_EQUAL: return VK_OEM_PLUS;      // =
        case KEY_LEFT_BRACKET: return VK_OEM_4;  // [
        case KEY_RIGHT_BRACKET: return VK_OEM_6; // ]
        case KEY_BACKSLASH: return VK_OEM_5;     // \
        case KEY_SEMICOLON: return VK_OEM_1;     // ;
        case KEY_APOSTROPHE: return VK_OEM_7;    // '
        case KEY_COMMA: return VK_OEM_COMMA;     // ,
        case KEY_PERIOD: return VK_OEM_PERIOD;   // .
        case KEY_SLASH: return VK_OEM_2;         // /

        // Numpad keys
        case KEY_NUMPAD_0: return VK_NUMPAD0;
        case KEY_NUMPAD_1: return VK_NUMPAD1;
        case KEY_NUMPAD_2: return VK_NUMPAD2;
        case KEY_NUMPAD_3: return VK_NUMPAD3;
        case KEY_NUMPAD_4: return VK_NUMPAD4;
        case KEY_NUMPAD_5: return VK_NUMPAD5;
        case KEY_NUMPAD_6: return VK_NUMPAD6;
        case KEY_NUMPAD_7: return VK_NUMPAD7;
        case KEY_NUMPAD_8: return VK_NUMPAD8;
        case KEY_NUMPAD_9: return VK_NUMPAD9;
        case KEY_NUMPAD_DECIMAL: return VK_DECIMAL;
        case KEY_NUMPAD_DIVIDE: return VK_DIVIDE;
        case KEY_NUMPAD_MULTIPLY: return VK_MULTIPLY;
        case KEY_NUMPAD_SUBTRACT: return VK_SUBTRACT;
        case KEY_NUMPAD_ADD: return VK_ADD;
        case KEY_NUMPAD_ENTER: return VK_RETURN;  // Same as main Enter key
        case KEY_NUMPAD_EQUAL: return VK_OEM_PLUS; // Approximation

        // Lock keys
        case KEY_NUM_LOCK: return VK_NUMLOCK;
        case KEY_SCROLL_LOCK: return VK_SCROLL;
        case KEY_PRINT_SCREEN: return VK_SNAPSHOT;
        case KEY_PAUSE: return VK_PAUSE;

        // Media keys
        case KEY_MUTE: return VK_VOLUME_MUTE;
        case KEY_VOLUME_UP: return VK_VOLUME_UP;
        case KEY_VOLUME_DOWN: return VK_VOLUME_DOWN;
        case KEY_MEDIA_PLAY_PAUSE: return VK_MEDIA_PLAY_PAUSE;
        case KEY_MEDIA_STOP: return VK_MEDIA_STOP;
        case KEY_MEDIA_NEXT: return VK_MEDIA_NEXT_TRACK;
        case KEY_MEDIA_PREVIOUS: return VK_MEDIA_PREV_TRACK;

        // Miscellaneous
        case KEY_MENU: return VK_APPS;

        default: return 0;
    }
}
