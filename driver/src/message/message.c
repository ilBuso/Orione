#include "message.h"

#include <stdio.h>
#include <unistd.h>
#include <linux/uinput.h>
#include <stdlib.h>

volatile Communication comm_step = INIT;

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

Fragment* receive_fragment(int serial_fd, Communication current_step) {
    Fragment* fragment = (Fragment*)malloc(sizeof(Fragment));
    if (fragment == NULL) {
        return NULL;
    }
    
    bool received_fragment_type = false;
    bool received_fragment_data = false;
    
    for (int i = 0; !received_fragment_type || !received_fragment_data; i++) {
        // Don't know if it is enough or what but is a good way to not do busy waiting inside a function
        if (i >= MAX_WAIT_CYCLES) {
            return NULL;
        }

        uint8_t packet = receive_packet(serial_fd);
        if (packet != 255) {
            if (comm_step == current_step) {
                if (!received_fragment_type) {
                    fragment->fragment_type = (FrgType)packet;
                    received_fragment_type = true;
                } else {
                    free(fragment);
                    return NULL;
                }
            } else if (comm_step == current_step + 1) {
                if (!received_fragment_data) {
                    fragment->data = packet;
                    received_fragment_data = true;
                } else {
                    free(fragment);
                    return NULL;
                }
            } else {
                free(fragment);
                return NULL;
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
    
    Fragment* x_fragment = NULL;
    Fragment* y_fragment = NULL;
    Fragment* info_fragment = NULL;
    
    if (comm_step == INIT) {
        x_fragment = receive_fragment(serial_fd, X_TYPE);
        if (x_fragment == NULL) {
            free(msg);
            return NULL;
        }
    } else {
        free(msg);
        return NULL;
    }
    
    if (comm_step == X_DATA) {
        y_fragment = receive_fragment(serial_fd, Y_TYPE);
        if (y_fragment == NULL) {
            free(x_fragment);
            free(msg);
            return NULL;
        }
    } else {
        free(x_fragment);
        free(msg);
        return NULL;
    }
    
    if (comm_step == Y_DATA) {
        info_fragment = receive_fragment(serial_fd, INFO_TYPE);
        if (info_fragment == NULL) {
            free(x_fragment);
            free(y_fragment);
            free(msg);
            return NULL;
        }
    } else {
        free(x_fragment);
        free(y_fragment);
        free(msg);
        return NULL;
    }
    
    msg->x = *x_fragment;
    msg->y = *y_fragment;
    msg->info = *info_fragment;
    
    free(x_fragment);
    free(y_fragment);
    free(info_fragment);
    
    return msg;
}
