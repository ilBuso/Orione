#include "message.h"

#include <stdio.h>
#include <stdlib.h>

Message* new_message(MsgType msg_type, uint8_t data) {
    Message *msg = (Message*) malloc(sizeof(Message));
    if (msg == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    
    msg->msg_type = msg_type;
    msg->data = data;
    
    return msg;
}

bool send_message(Message* msg, RGB rgb) {
    if (msg == NULL) {
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
    UART_transmitData(EUSCI_A2_BASE, msg->msg_type);
    // Wait for flag to reset
    while (UART_queryStatusFlags(EUSCI_A2_BASE, EUSCI_A_UART_BUSY));

    // Send data
    UART_transmitData(EUSCI_A2_BASE, msg->data);
    // Wait for flag to reset
    while (UART_queryStatusFlags(EUSCI_A2_BASE, EUSCI_A_UART_BUSY));
    
    free(msg);
    return true;
}

void send_full_message(Message x, Message y, Message info) {
    if (!send_message(x, RED)) {
        // ¿ error handling ?
    } else if (!send_message(y, GREEN)) {
        // ¿ error handling ?
    }else if (!send_message(info, BLUE)) {
        // ¿ error handling ?
    }
}