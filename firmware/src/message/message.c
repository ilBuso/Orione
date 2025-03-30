#include "message.h"

#include <stdio.h>
#include <stdlib.h>

Fragment* new_fragment(FrgType fragment_type, uint8_t data) {
    Fragment *fragment = (Fragment*) malloc(sizeof(Fragment));
    if (fragment == NULL) {
        return NULL;
    }
    
    fragment->fragment_type = fragment_type;
    fragment->data = data;
    
    return fragment;
}

Message* new_message(Fragment x, Fragment y, Fragment info) {
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

    // Toggle LED for visual feedback
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
    
    // Send MsgType
    UART_transmitData(EUSCI_A2_BASE, fragment->fragment_type);
    // Wait for flag to reset
    while (UART_queryStatusFlags(EUSCI_A2_BASE, EUSCI_A_UART_BUSY));

    // Send data
    UART_transmitData(EUSCI_A2_BASE, fragment->data);
    // Wait for flag to reset
    while (UART_queryStatusFlags(EUSCI_A2_BASE, EUSCI_A_UART_BUSY));
    
    free(fragment);
    return true;
}

bool send_message(Message* msg) {
    if (msg == NULL) {
        return false;
    }
    
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
    
    bool success = true;    
    if (!send_fragment(x_frag, RED)) {
        success = false;
    } else if (!send_fragment(y_frag, GREEN)) {
        success = false;
    } else if (!send_fragment(info_frag, BLUE)) {
        success = false;
    }
        
    free(msg);
    return success;
}