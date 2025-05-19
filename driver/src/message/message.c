#include "message.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <core/io/uart.h>

// Global state tracking the current step in the communication protocol
volatile Communication comm_step = INIT;

Fragment* receive_fragment(Communication current_step) {
    // Allocate memory for a new fragment
    Fragment* fragment = (Fragment*)malloc(sizeof(Fragment));
    if (fragment == NULL) {
        // Return NULL if memory allocation failed
        return NULL;
    }
    
    // Track reception status of fragment parts
    bool received_fragment_type = false;
    bool received_fragment_data = false;
    
    // Try to receive both fragment type and data
    for (int i = 0; !received_fragment_type || !received_fragment_data; i++) {
        // Timeout mechanism to prevent infinite loops
        if (i >= MAX_WAIT_CYCLES) {
            return NULL;
        }
        
        // Get a packet from the serial connection
        uint8_t packet = uart_read_byte();
        
        // If we received valid data
        if (packet != 255) {
            if (comm_step == current_step) {
                // First packet should contain fragment type
                if (!received_fragment_type) {
                    fragment->fragment_type = (FrgType)packet;
                    received_fragment_type = true;
                } else {
                    // Protocol error - already received type when expecting type
                    free(fragment);
                    return NULL;
                }
            } else if (comm_step == current_step + 1) {
                // Second packet should contain fragment data
                if (!received_fragment_data) {
                    fragment->data = packet;
                    received_fragment_data = true;
                } else {
                    // Protocol error - already received data when expecting data
                    free(fragment);
                    return NULL;
                }
            } else {
                // Communication step mismatch - protocol error
                free(fragment);
                return NULL;
            }
        } else {
            // Failed to receive valid data
            free(fragment);
            return NULL;
        }
    }
    
    // Successfully received both type and data
    return fragment;
}

Message* receive_message() {
    // Allocate memory for a new message
    Message* msg = (Message*)malloc(sizeof(Message));
    if (msg == NULL) {
        return NULL;
    }
    
    Fragment* x_fragment = NULL;
    Fragment* y_fragment = NULL;
    Fragment* info_fragment = NULL;
    
    // A complete message consists of three fragments: X, Y, and INFO
    // First, receive X fragment
    if (comm_step == INIT) {
        x_fragment = receive_fragment(X_TYPE);
        if (x_fragment == NULL) {
            free(msg);
            return NULL;
        }
    } else {
        // Protocol error - not at INIT state when starting message reception
        free(msg);
        return NULL;
    }
    
    // Next, receive Y fragment
    if (comm_step == X_DATA) {
        y_fragment = receive_fragment(Y_TYPE);
        if (y_fragment == NULL) {
            free(x_fragment);
            free(msg);
            return NULL;
        }
    } else {
        // Protocol error - not at X_DATA state when expecting Y fragment
        free(x_fragment);
        free(msg);
        return NULL;
    }
    
    // Finally, receive INFO fragment
    if (comm_step == Y_DATA) {
        info_fragment = receive_fragment(INFO_TYPE);
        if (info_fragment == NULL) {
            free(x_fragment);
            free(y_fragment);
            free(msg);
            return NULL;
        }
    } else {
        // Protocol error - not at Y_DATA state when expecting INFO fragment
        free(x_fragment);
        free(y_fragment);
        free(msg);
        return NULL;
    }
    
    // Copy fragment data into message structure
    msg->x = *x_fragment;
    msg->y = *y_fragment;
    msg->info = *info_fragment;
    
    // Free the fragment containers but keep their data in the message
    free(x_fragment);
    free(y_fragment);
    free(info_fragment);
    
    return msg;
}
