#ifndef MATRIX_H
#define MATRIX_H

    #include <stdint.h>
    #include <windows.h>
    
    const int8_t key[6][17] = {
        {VK_ESCAPE, VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12, VK_SNAPSHOT, VK_SCROLL, VK_PAUSE, 0},
        {VK_OEM_3, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', VK_OEM_MINUS, VK_OEM_PLUS, VK_BACK, VK_INSERT, VK_HOME, VK_PRIOR},
        {VK_TAB, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', VK_OEM_4, VK_OEM_6, VK_RETURN, VK_DELETE, VK_END, VK_NEXT},
        {VK_CAPITAL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', VK_OEM_1, VK_OEM_7, VK_OEM_5, 0, 0, 0, 0},
        {VK_LSHIFT, VK_OEM_102, 'Z', 'X', 'C', 'V', 'B', 'N', 'M', VK_OEM_COMMA, VK_OEM_PERIOD, VK_OEM_2, VK_RSHIFT, 0, VK_UP, 0, 0},
        {VK_LCONTROL, VK_LWIN, VK_LMENU, 0, 0, VK_SPACE, 0, 0, 0, VK_RMENU, VK_RWIN, VK_APPS, VK_RCONTROL, VK_LEFT, VK_DOWN, VK_RIGHT, 0}
      }

#endif // MATRIX_H