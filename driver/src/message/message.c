#include "message.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <stdlib.h>

volatile uint8_t comm_step = INIT;

uint8_t receive_packet(int serial_fd) {
    unsigned char buffer[1];
    ssize_t bytes_read = read(serial_fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        comm_step++;
        return (uint8_t)buffer[0];
    } else {
        return 255;
    }
}

Fragment* receive_fragment(int serial_fd) {
    Fragment* fragment = (Fragment*)malloc(sizeof(Fragment));
    if (fragment == NULL) {
        return NULL;
    }
    
    bool received_fragment_type = false;
    bool received_fragment_data = false;
    
    for (int i = 0; !received_fragment_type || !received_fragment_data; i++) {
        // Don't know if it is enough or what but is a good way to not do busy waiting inside a function
        if (i >= 500) {
            return NULL;
        }

        uint8_t packet = receive_packet(serial_fd);
        if (packet != 255) {
            if (comm_step % 2 != 0) {
                if (!received_fragment_type) {
                    fragment->fragment_type = (FrgType)packet;
                    received_fragment_type = true;
                } else {
                    free(fragment);
                    return NULL;
                }
            } else {
                if (!received_fragment_data) {
                    fragment->data = packet;
                    received_fragment_data = true;
                } else {
                    free(fragment);
                    return NULL;
                }
            }
        } else {
            free(fragment);
            return NULL;
        }
    }
    return fragment;
}

Message* receive_message(int serial_fd) {
    Message* msg = (Message*)malloc(sizeof(Message));
    if (msg == NULL) {
        return NULL;
    }
    
    Fragment* x = NULL;
    Fragment* y = NULL;
    Fragment* info = NULL;
    
    if (comm_step == INIT) {
        x = receive_fragment(serial_fd);
        if (x == NULL) {
            free(msg);
            return NULL;
        }
    } else {
        free(msg);
        return NULL;
    }
    
    if (comm_step == X_DATA) {
        y = receive_fragment(serial_fd);
        if (y == NULL) {
            free(x);
            free(msg);
            return NULL;
        }
    } else {
        free(x);
        free(msg);
        return NULL;
    }
    
    if (comm_step == Y_DATA) {
        info = receive_fragment(serial_fd);
        if (info == NULL) {
            free(x);
            free(y);
            free(msg);
            return NULL;
        }
    } else {
        free(x);
        free(y);
        free(msg);
        return NULL;
    }
    
    msg->x = *x;
    msg->y = *y;
    msg->info = *info;
    
    free(x);
    free(y);
    free(info);
    
    return msg;
}
