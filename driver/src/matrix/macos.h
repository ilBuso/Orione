#ifndef MATRIX_H
#define MATRIX_H

    #include <stdint.h>
    #include <Carbon/Carbon.h> 
    
    const int8_t key[6][17] = {
        {kVK_Escape, kVK_F1, kVK_F2, kVK_F3, kVK_F4, kVK_F5, kVK_F6, kVK_F7, kVK_F8, kVK_F9, kVK_F10, kVK_F11, kVK_F12, kVK_F13, kVK_F14, kVK_F15, 0},
        {kVK_ANSI_Grave, kVK_ANSI_1, kVK_ANSI_2, kVK_ANSI_3, kVK_ANSI_4, kVK_ANSI_5, kVK_ANSI_6, kVK_ANSI_7, kVK_ANSI_8, kVK_ANSI_9, kVK_ANSI_0, kVK_ANSI_Minus, kVK_ANSI_Equal, kVK_Delete, kVK_ForwardDelete, kVK_Home, kVK_PageUp},
        {kVK_Tab, kVK_ANSI_Q, kVK_ANSI_W, kVK_ANSI_E, kVK_ANSI_R, kVK_ANSI_T, kVK_ANSI_Y, kVK_ANSI_U, kVK_ANSI_I, kVK_ANSI_O, kVK_ANSI_P, kVK_ANSI_LeftBracket, kVK_ANSI_RightBracket, kVK_Return, kVK_ForwardDelete, kVK_End, kVK_PageDown},
        {kVK_CapsLock, kVK_ANSI_A, kVK_ANSI_S, kVK_ANSI_D, kVK_ANSI_F, kVK_ANSI_G, kVK_ANSI_H, kVK_ANSI_J, kVK_ANSI_K, kVK_ANSI_L, kVK_ANSI_Semicolon, kVK_ANSI_Quote, kVK_ANSI_Backslash, 0, 0, 0, 0},
        {kVK_Shift, kVK_ISO_Section, kVK_ANSI_Z, kVK_ANSI_X, kVK_ANSI_C, kVK_ANSI_V, kVK_ANSI_B, kVK_ANSI_N, kVK_ANSI_M, kVK_ANSI_Comma, kVK_ANSI_Period, kVK_ANSI_Slash, kVK_RightShift, 0, kVK_UpArrow, 0, 0},
        {kVK_Control, kVK_Option, kVK_Command, 0, 0, kVK_Space, 0, 0, 0, kVK_RightOption, kVK_RightCommand, 0, kVK_RightControl, kVK_LeftArrow, kVK_DownArrow, kVK_RightArrow, 0}
    }

#endif // MATRIX_H