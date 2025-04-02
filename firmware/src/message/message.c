#include "message.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Fragment* new_fragment(FrgType fragment_type, uint8_t data) {
    // Allocate memory for the fragment
    Fragment *fragment = (Fragment*) malloc(sizeof(Fragment));
    if (fragment == NULL) {
        return NULL;
    }
    
    fragment->fragment_type = fragment_type;
    fragment->data = data;

    return fragment;
}

Message* new_message(Fragment x, Fragment y, Fragment info) {
    // Allocate memory for the message
    Message *msg = (Message*) malloc(sizeof(Message));
    if (msg == NULL) {
        return NULL;
    }

    msg->x = x;
    msg->y = y;
    msg->info = info;

    return msg;
}

bool send_fragment(Fragment* fragment, RGB rgb) {
    if (fragment == NULL) {
        return false;
    }

    // Toggle LED based on RGB color for visual feedback
    switch (rgb) {
        case RED:
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);
            break;
        case GREEN:
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
            break;
        case BLUE:
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
            break;
    }
    
    // Send fragment type over UART
    UART_transmitData(EUSCI_A2_BASE, fragment->fragment_type);
    if (!UART_wait(EUSCI_A2_BASE)) {
        return false;
    }

    // Send fragment data over UART
    UART_transmitData(EUSCI_A2_BASE, fragment->data);
    if (!UART_wait(EUSCI_A2_BASE)) {
        return false;
    }
    
    // Free the fragment memory after sending
    free(fragment);
    return true;
}

bool send_message(Message* msg) {
    if (msg == NULL) {
        return false;
    }
    
    // Create copies of the fragments for transmission
    Fragment* x_frag = new_fragment(msg->x.fragment_type, msg->x.data);
    if (x_frag == NULL) {
        return false;
    }
    
    Fragment* y_frag = new_fragment(msg->y.fragment_type, msg->y.data);
    if (y_frag == NULL) {
        free(x_frag);
        return false;
    }
    
    Fragment* info_frag = new_fragment(msg->info.fragment_type, msg->info.data);
    if (info_frag == NULL) {
        free(x_frag);
        free(y_frag);
        return false;
    }
    
    // Send each fragment with appropriate color indication
    bool success = true;
    if (!send_fragment(x_frag, RED)) {
        success = false;
    } else if (!send_fragment(y_frag, GREEN)) {
        success = false;
    } else if (!send_fragment(info_frag, BLUE)) {
        success = false;
    }

    // Free the message memory after sending
    free(msg);
    return success;
}

// Waits for UART to be ready with timeout protection
bool UART_wait(uint32_t module_instance) {
    int attempts = 0;
    for (; UART_queryStatusFlags(module_instance, EUSCI_A_UART_BUSY), attempts++) {
        if (attempts >= UART_MAX_ATTEMPTS) {
            return false;
        }
    }
    return true;
}