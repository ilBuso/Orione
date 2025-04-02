#ifndef EMULATOR_H
#define EMULATOR_H

    #include "../message/message.h"
    #include "../matrix/linux.h"
    #include "../global/global.h"

    #include <stdio.h>
    #include <unistd.h>

    inline void emulate_key(Message* msg, int uinput_fd) {
        // to review based on how the coordinates are sent
        if (msg->x.data <= NUM_COLS && msg->y.data <= NUM_ROWS) {
            int8_t keycode = key[msg->x.data][msg->y.data];
    
            if (keycode > 0) {
                // Simulate key press
                struct input_event ev = {};
                ev.type = EV_KEY;
                ev.code = keycode;
                ev.value = 1; // Key press
                write(uinput_fd, &ev, sizeof(ev));
    
                // Simulate key release
                ev.value = 0; // Key release
                write(uinput_fd, &ev, sizeof(ev));
    
                // Synchronize
                ev.type = EV_SYN;
                ev.code = SYN_REPORT;
                ev.value = 0;
                write(uinput_fd, &ev, sizeof(ev));
    
                printf("Key pressed: %c (keycode: %d)\n", keycode, keycode);
            } else {
                printf("Unknown keycode received: %d\n", keycode);
            }
        }
    }
#endif // EMULATOR_H