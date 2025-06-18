//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#include "core/io/input_device.h"
#include "core/logging.h"
#include "matrix/keycodes.h"

#include <_stdlib.h>
#include <Carbon/Carbon.h>

int input_device_init()
{
    // we don't create anything via HID for macOS
    LOG_DEBUG("macOS input device initialized");
    return EXIT_SUCCESS;
}

void input_device_cleanup()
{
    // nothing to do
    LOG_DEBUG("macOS input device cleaned up");
}

void input_device_send_key(const enum CrossPlatformKeyCode key_code, bool pressed)
{
    LOG_DEBUG("macOS input device sending key %d with pressed %d", key_code, pressed);
}

CGKeyCode convertToMacOSKeyCode(enum CrossPlatformKeyCode keyCode) {
    switch (keyCode) {
        // Alphabetic keys
        case KEY_A: return kVK_ANSI_A;
        case KEY_B: return kVK_ANSI_B;
        case KEY_C: return kVK_ANSI_C;
        case KEY_D: return kVK_ANSI_D;
        case KEY_E: return kVK_ANSI_E;
        case KEY_F: return kVK_ANSI_F;
        case KEY_G: return kVK_ANSI_G;
        case KEY_H: return kVK_ANSI_H;
        case KEY_I: return kVK_ANSI_I;
        case KEY_J: return kVK_ANSI_J;
        case KEY_K: return kVK_ANSI_K;
        case KEY_L: return kVK_ANSI_L;
        case KEY_M: return kVK_ANSI_M;
        case KEY_N: return kVK_ANSI_N;
        case KEY_O: return kVK_ANSI_O;
        case KEY_P: return kVK_ANSI_P;
        case KEY_Q: return kVK_ANSI_Q;
        case KEY_R: return kVK_ANSI_R;
        case KEY_S: return kVK_ANSI_S;
        case KEY_T: return kVK_ANSI_T;
        case KEY_U: return kVK_ANSI_U;
        case KEY_V: return kVK_ANSI_V;
        case KEY_W: return kVK_ANSI_W;
        case KEY_X: return kVK_ANSI_X;
        case KEY_Y: return kVK_ANSI_Y;
        case KEY_Z: return kVK_ANSI_Z;

        // Numeric keys
        case KEY_0: return kVK_ANSI_0;
        case KEY_1: return kVK_ANSI_1;
        case KEY_2: return kVK_ANSI_2;
        case KEY_3: return kVK_ANSI_3;
        case KEY_4: return kVK_ANSI_4;
        case KEY_5: return kVK_ANSI_5;
        case KEY_6: return kVK_ANSI_6;
        case KEY_7: return kVK_ANSI_7;
        case KEY_8: return kVK_ANSI_8;
        case KEY_9: return kVK_ANSI_9;

        // Function keys
        case KEY_F1: return kVK_F1;
        case KEY_F2: return kVK_F2;
        case KEY_F3: return kVK_F3;
        case KEY_F4: return kVK_F4;
        case KEY_F5: return kVK_F5;
        case KEY_F6: return kVK_F6;
        case KEY_F7: return kVK_F7;
        case KEY_F8: return kVK_F8;
        case KEY_F9: return kVK_F9;
        case KEY_F10: return kVK_F10;
        case KEY_F11: return kVK_F11;
        case KEY_F12: return kVK_F12;

        // Modifier keys
        case KEY_SHIFT_LEFT: return kVK_Shift;
        case KEY_SHIFT_RIGHT: return kVK_RightShift;
        case KEY_CONTROL_LEFT: return kVK_Control;
        case KEY_CONTROL_RIGHT: return kVK_RightControl;
        case KEY_ALT_LEFT: return kVK_Option;
        case KEY_ALT_RIGHT: return kVK_RightOption;
        case KEY_SUPER_LEFT: return kVK_Command;
        case KEY_SUPER_RIGHT: return kVK_RightCommand;

        // Navigation keys
        case KEY_ESCAPE: return kVK_Escape;
        case KEY_TAB: return kVK_Tab;
        case KEY_CAPS_LOCK: return kVK_CapsLock;
        case KEY_ENTER: return kVK_Return;
        case KEY_BACKSPACE: return kVK_Delete;
        case KEY_INSERT: return 0x72; // Not standard on Mac keyboards
        case KEY_DELETE: return kVK_ForwardDelete;
        case KEY_HOME: return kVK_Home;
        case KEY_END: return kVK_End;
        case KEY_PAGE_UP: return kVK_PageUp;
        case KEY_PAGE_DOWN: return kVK_PageDown;
        case KEY_UP: return kVK_UpArrow;
        case KEY_DOWN: return kVK_DownArrow;
        case KEY_LEFT: return kVK_LeftArrow;
        case KEY_RIGHT: return kVK_RightArrow;

        // Special character keys
        case KEY_SPACE: return kVK_Space;
        case KEY_GRAVE_ACCENT: return kVK_ANSI_Grave;
        case KEY_MINUS: return kVK_ANSI_Minus;
        case KEY_EQUAL: return kVK_ANSI_Equal;
        case KEY_LEFT_BRACKET: return kVK_ANSI_LeftBracket;
        case KEY_RIGHT_BRACKET: return kVK_ANSI_RightBracket;
        case KEY_BACKSLASH: return kVK_ANSI_Backslash;
        case KEY_SEMICOLON: return kVK_ANSI_Semicolon;
        case KEY_APOSTROPHE: return kVK_ANSI_Quote;
        case KEY_COMMA: return kVK_ANSI_Comma;
        case KEY_PERIOD: return kVK_ANSI_Period;
        case KEY_SLASH: return kVK_ANSI_Slash;

        // Numpad keys
        case KEY_NUMPAD_0: return kVK_ANSI_Keypad0;
        case KEY_NUMPAD_1: return kVK_ANSI_Keypad1;
        case KEY_NUMPAD_2: return kVK_ANSI_Keypad2;
        case KEY_NUMPAD_3: return kVK_ANSI_Keypad3;
        case KEY_NUMPAD_4: return kVK_ANSI_Keypad4;
        case KEY_NUMPAD_5: return kVK_ANSI_Keypad5;
        case KEY_NUMPAD_6: return kVK_ANSI_Keypad6;
        case KEY_NUMPAD_7: return kVK_ANSI_Keypad7;
        case KEY_NUMPAD_8: return kVK_ANSI_Keypad8;
        case KEY_NUMPAD_9: return kVK_ANSI_Keypad9;
        case KEY_NUMPAD_DECIMAL: return kVK_ANSI_KeypadDecimal;
        case KEY_NUMPAD_DIVIDE: return kVK_ANSI_KeypadDivide;
        case KEY_NUMPAD_MULTIPLY: return kVK_ANSI_KeypadMultiply;
        case KEY_NUMPAD_SUBTRACT: return kVK_ANSI_KeypadMinus;
        case KEY_NUMPAD_ADD: return kVK_ANSI_KeypadPlus;
        case KEY_NUMPAD_ENTER: return kVK_ANSI_KeypadEnter;
        case KEY_NUMPAD_EQUAL: return kVK_ANSI_KeypadEquals;

        // Lock keys
        case KEY_NUM_LOCK: return kVK_ANSI_KeypadClear;  // Closest equivalent
        case KEY_SCROLL_LOCK: return 0xE0;  // Not standard on Mac
        case KEY_PRINT_SCREEN: return 0xE1; // Not standard on Mac
        case KEY_PAUSE: return 0xE2;        // Not standard on Mac

        // Media keys
        case KEY_MUTE: return kVK_Mute;
        case KEY_VOLUME_UP: return kVK_VolumeUp;
        case KEY_VOLUME_DOWN: return kVK_VolumeDown;
        case KEY_MEDIA_PLAY_PAUSE: return kVK_F8;  // Media key on Mac F8
        case KEY_MEDIA_STOP: return kVK_F7;        // Media key on Mac F7
        case KEY_MEDIA_NEXT: return kVK_F9;        // Media key on Mac F9
        case KEY_MEDIA_PREVIOUS: return kVK_F7;    // Media key on Mac F7

        // Miscellaneous
        case KEY_MENU: return 0x6E;  // Context menu not standard on Mac

        default: return 0xFF;  // Invalid key code
    }
}