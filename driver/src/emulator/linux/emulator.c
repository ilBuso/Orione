#include "emulator.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>

int8_t read_bit(uint8_t packet, int bit_pos) {
    if (bit_pos < 0 || bit_pos > 7) {
        return -1;
    }

    return (packet >> bit_pos) & 1;
}

bool emulate_key(Message* msg, int uinput_fd, const Parse* parse) {
    // to review based on how the coordinates are sent
    if (msg->x.data <= NUM_COLS && msg->y.data <= NUM_ROWS) {

        // layer
        bool encoder;
        uint8_t layer;

        // check if is encoder or keyboard layer
        uint8_t keycode;
        int8_t encoder_flag = read_bit(msg->info.data, 0);
        if (encoder_flag != -1) {
            // is NOT encoder key
            if (encoder_flag == 0) {
                encoder = false;
            }
            // is encoder key
            if (encoder_flag == 1) {
                encoder = true;

                // get keycode
                keycode = parse_get_encoder(parse, msg->x.data);
            }
        } else {
            //error
        }

        // set key-state matrix
        int8_t pressed_flag = read_bit(msg->info.data, 0);
        if (pressed_flag != -1) {
            if (pressed_flag == 0) {
                // change key-state matrix
                if (encoder) {
                    // encoder
                } else {
                    // matrix
                }
            }
            if (pressed_flag == 1) {
                // change key-state matrix
                if (encoder) {
                    // encoder
                } else {
                    // matrix
                }
            }
        } else {
            //error
        }

        // if it is a normal layer
        if (!encoder) {
            // call function to get current layer
            //layer = 
            
            // get right keycode from parser
            //parse_get_layer(const Parse *parse, int layer_index, int row, int col)
        }

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
    }
    return true;
}
