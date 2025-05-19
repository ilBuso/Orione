//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#import "core/io/input_device.h"
#import "core/logging.c"
#include "matrix/keycodes.h"

// TODO: Check libs needed (remove unnecessary)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <termios.h>
#include <X11/keysym.h>

KeySym convertToLinuxKeyCode(enum CrossPlatformKeyCode keyCode);

static int uinput_fd = -1;

int input_device_init()
{
    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0)
    {
        LOG_ERROR("Error opening uinput device");
        return EXIT_FAILURE;
    }

    struct uinput_user_dev uidev = {};
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "MSP432 Virtual Keyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1234; // Vendor ID
    uidev.id.product = 0x5678; // Product ID
    uidev.id.version = 1;

    // Enable all the keys you want to simulate
    for (int i = 0; i < 256; i++)
    {
        ioctl(uinput_fd, UI_SET_KEYBIT, i);
    }
    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);

    // Write the device configuration
    if (write(uinput_fd, &uidev, sizeof(uidev)) < 0)
    {
        LOG_ERROR("Error setting up uinput device");
        close(uinput_fd);
        return EXIT_FAILURE;
    }
    if (ioctl(uinput_fd, UI_DEV_CREATE) < 0)
    {
        LOG_ERROR("Error creating uinput device");
        close(uinput_fd);
        return EXIT_FAILURE;
    }

    LOG_DEBUG("Linux input device setup");

    return EXIT_SUCCESS;
}

void input_device_send_key(const enum CrossPlatformKeyCode key_code, bool pressed)
{
    const KeySm key = convertToLinuxKeyCode(key_code);

    struct input_event ev = {};
    ev.type = EV_KEY;
    ev.code = key;
    if (pressed)
    {
        ev.value = 1;
    } else
    {
        ev.value = 0;
    }

    write(uinput_fd, &ev, sizeof(ev));

    // Synchronize
    ev.type = EV_SYN;
    ev.code = SYN_REPORT;
    ev.value = 0;
    write(uinput_fd, &ev, sizeof(ev));
}


void input_device_cleanup()
{
    if (uinput_fd >= 0)
    {
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
    }

    LOG_DEBUG("Linux input device cleaned up");
}

KeySym convertToLinuxKeyCode(enum CrossPlatformKeyCode keyCode) {
    switch (keyCode) {
        // Alphabetic keys
        case KEY_A: return XK_a;
        case KEY_B: return XK_b;
        case KEY_C: return XK_c;
        case KEY_D: return XK_d;
        case KEY_E: return XK_e;
        case KEY_F: return XK_f;
        case KEY_G: return XK_g;
        case KEY_H: return XK_h;
        case KEY_I: return XK_i;
        case KEY_J: return XK_j;
        case KEY_K: return XK_k;
        case KEY_L: return XK_l;
        case KEY_M: return XK_m;
        case KEY_N: return XK_n;
        case KEY_O: return XK_o;
        case KEY_P: return XK_p;
        case KEY_Q: return XK_q;
        case KEY_R: return XK_r;
        case KEY_S: return XK_s;
        case KEY_T: return XK_t;
        case KEY_U: return XK_u;
        case KEY_V: return XK_v;
        case KEY_W: return XK_w;
        case KEY_X: return XK_x;
        case KEY_Y: return XK_y;
        case KEY_Z: return XK_z;

        // Numeric keys
        case KEY_0: return XK_0;
        case KEY_1: return XK_1;
        case KEY_2: return XK_2;
        case KEY_3: return XK_3;
        case KEY_4: return XK_4;
        case KEY_5: return XK_5;
        case KEY_6: return XK_6;
        case KEY_7: return XK_7;
        case KEY_8: return XK_8;
        case KEY_9: return XK_9;

        // Function keys
        case KEY_F1: return XK_F1;
        case KEY_F2: return XK_F2;
        case KEY_F3: return XK_F3;
        case KEY_F4: return XK_F4;
        case KEY_F5: return XK_F5;
        case KEY_F6: return XK_F6;
        case KEY_F7: return XK_F7;
        case KEY_F8: return XK_F8;
        case KEY_F9: return XK_F9;
        case KEY_F10: return XK_F10;
        case KEY_F11: return XK_F11;
        case KEY_F12: return XK_F12;

        // Modifier keys
        case KEY_SHIFT_LEFT: return XK_Shift_L;
        case KEY_SHIFT_RIGHT: return XK_Shift_R;
        case KEY_CONTROL_LEFT: return XK_Control_L;
        case KEY_CONTROL_RIGHT: return XK_Control_R;
        case KEY_ALT_LEFT: return XK_Alt_L;
        case KEY_ALT_RIGHT: return XK_Alt_R;
        case KEY_SUPER_LEFT: return XK_Super_L;
        case KEY_SUPER_RIGHT: return XK_Super_R;

        // Navigation keys
        case KEY_ESCAPE: return XK_Escape;
        case KEY_TAB: return XK_Tab;
        case KEY_CAPS_LOCK: return XK_Caps_Lock;
        case KEY_ENTER: return XK_Return;
        case KEY_BACKSPACE: return XK_BackSpace;
        case KEY_INSERT: return XK_Insert;
        case KEY_DELETE: return XK_Delete;
        case KEY_HOME: return XK_Home;
        case KEY_END: return XK_End;
        case KEY_PAGE_UP: return XK_Page_Up;
        case KEY_PAGE_DOWN: return XK_Page_Down;
        case KEY_UP: return XK_Up;
        case KEY_DOWN: return XK_Down;
        case KEY_LEFT: return XK_Left;
        case KEY_RIGHT: return XK_Right;

        // Special character keys
        case KEY_SPACE: return XK_space;
        case KEY_GRAVE_ACCENT: return XK_grave;
        case KEY_MINUS: return XK_minus;
        case KEY_EQUAL: return XK_equal;
        case KEY_LEFT_BRACKET: return XK_bracketleft;
        case KEY_RIGHT_BRACKET: return XK_bracketright;
        case KEY_BACKSLASH: return XK_backslash;
        case KEY_SEMICOLON: return XK_semicolon;
        case KEY_APOSTROPHE: return XK_apostrophe;
        case KEY_COMMA: return XK_comma;
        case KEY_PERIOD: return XK_period;
        case KEY_SLASH: return XK_slash;

        // Numpad keys
        case KEY_NUMPAD_0: return XK_KP_0;
        case KEY_NUMPAD_1: return XK_KP_1;
        case KEY_NUMPAD_2: return XK_KP_2;
        case KEY_NUMPAD_3: return XK_KP_3;
        case KEY_NUMPAD_4: return XK_KP_4;
        case KEY_NUMPAD_5: return XK_KP_5;
        case KEY_NUMPAD_6: return XK_KP_6;
        case KEY_NUMPAD_7: return XK_KP_7;
        case KEY_NUMPAD_8: return XK_KP_8;
        case KEY_NUMPAD_9: return XK_KP_9;
        case KEY_NUMPAD_DECIMAL: return XK_KP_Decimal;
        case KEY_NUMPAD_DIVIDE: return XK_KP_Divide;
        case KEY_NUMPAD_MULTIPLY: return XK_KP_Multiply;
        case KEY_NUMPAD_SUBTRACT: return XK_KP_Subtract;
        case KEY_NUMPAD_ADD: return XK_KP_Add;
        case KEY_NUMPAD_ENTER: return XK_KP_Enter;
        case KEY_NUMPAD_EQUAL: return XK_KP_Equal;

        // Lock keys
        case KEY_NUM_LOCK: return XK_Num_Lock;
        case KEY_SCROLL_LOCK: return XK_Scroll_Lock;
        case KEY_PRINT_SCREEN: return XK_Print;
        case KEY_PAUSE: return XK_Pause;

        // Media keys
        case KEY_MUTE: return XF86XK_AudioMute;
        case KEY_VOLUME_UP: return XF86XK_AudioRaiseVolume;
        case KEY_VOLUME_DOWN: return XF86XK_AudioLowerVolume;
        case KEY_MEDIA_PLAY_PAUSE: return XF86XK_AudioPlay;
        case KEY_MEDIA_STOP: return XF86XK_AudioStop;
        case KEY_MEDIA_NEXT: return XF86XK_AudioNext;
        case KEY_MEDIA_PREVIOUS: return XF86XK_AudioPrev;

        // Miscellaneous
        case KEY_MENU: return XK_Menu;

        default: return XK_VoidSymbol;
    }
}