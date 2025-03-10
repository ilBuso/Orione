#include "interrupt.h"

void PORT2_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < PORT_2_NUM_PINS; i++) {
        if(status & port_2_columns[i][0]) {
            scan_rows(GPIO_PORT_P2, port_2_columns[i][0]);
            break;
        }
    }
}

void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < PORT_3_NUM_PINS; i++) {
        if(status & port_3_columns[i][0]) {
            scan_rows(GPIO_PORT_P3, port_3_columns[i][0]);
            break;
        }
    }
}

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < PORT_4_NUM_PINS; i++) {
        if(status & port_4_columns[i][0]) {
            scan_rows(GPIO_PORT_P4, port_4_columns[i][0]);
            break;
        }
    }
}

void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < PORT_5_NUM_PINS; i++) {
        if(status & port_5_columns[i][0]) {
            scan_rows(GPIO_PORT_P5, port_5_columns[i][0]);
            break;
        }
    }
}

void PORT6_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < PORT_6_NUM_PINS; i++) {
        if(status & port_6_columns[i][0]) {
            scan_rows(GPIO_PORT_P6, port_6_columns[i][0]);
            break;
        }
    }
}
