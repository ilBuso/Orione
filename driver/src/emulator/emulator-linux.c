#include "emulator-linux.h"

#include "../parser/parser.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>

extern Matrix* matrix;

bool emulate_key(Message* msg, int uinput_fd) {
    // to review based on how the coordinates are sent
    if (msg->x.data <= NUM_COLS && msg->y.data <= NUM_ROWS) {
        //int8_t keycode = key[msg->x.data][msg->y.data];

        int8_t keycode = matrix_get(matrix, msg->x.data, msg->y.data);
        if (keycode != KEY_RESERVED) {
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

            fprintf(stderr, "Key pressed: %c (keycode: %d)\n", keycode, keycode);
        } else {
            fprintf(stderr, "Unknown keycode received: %d\n", keycode);
        }

        matrix_free(matrix);
    }
    return true;
}
