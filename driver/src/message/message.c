#include "message.h"
#include "../matrix/linux.h"
#include "../global/global.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>

volatile uint8_t comm_step = INIT;

Message receive_message_linux(int serial_fd) {
    Message msg;
    for (int index = 0; comm_step != END; index++) {
        unsigned char buffer[1];
        ssize_t bytes_read = read(serial_fd, buffer, sizeof(buffer));

        if (bytes_read > 0) {
            if (index % 2 != 0) {
                // message type
                uint8_t msg_type = buffer[0];
                switch (msg_type) {
                    case INFO:
                        // check previous step
                        if (comm_step != Y_DATA) {
                            // error
                        }
                        comm_step = INFO_MSG_TYPE;
                        break;
                    case X:
                        // check previous step
                        if (comm_step != INIT) {
                            // error
                        }
                        comm_step = X_MSG_TYPE;
                        break;
                    case Y:
                        // check previous step
                        if (comm_step != X_DATA) {
                            // error
                        }
                        comm_step = Y_MSG_TYPE;
                        break;
                    case END:
                        return msg;
                        break;
                    default:
                        // error
                        break;
                }
            } else {
                // data
                switch (comm_step) {
                    case INFO_MSG_TYPE:
                        comm_step = INFO_DATA;
                        msg.info = buffer[0];
                        break;
                    case X_MSG_TYPE:
                        comm_step = X_DATA;
                        msg.x = buffer[0];
                        break;
                    case Y_MSG_TYPE:
                        comm_step = X_DATA;
                        msg.y = buffer[0];
                        break;
                    default:
                        // error
                        break;
                }
            }
        }
    }
}

Message receive_message_macos() {

}

Message receive_message_win64() {

}

void emulate_key_linux(Message msg, int uinput_fd) {
    // to review based on how the coordinates are sent
    if (msg.x <= NUM_COLS && msg.y <= NUM_ROWS) {
        int8_t keycode = key[msg.x][msg.y];

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

void emulate_key_macos() {

}

void emulate_key_win64() {
    
}