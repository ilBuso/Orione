#ifndef GLOBAL_H
#define GLOBAL_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    #include <stdint.h>

    #define NUM_ROWS 6
    #define NUM_COLS 17
    #define DEBOUNCE_THRESHOLD_MS 20

    #define PORT_2_NUM_PINS 4
    #define PORT_3_NUM_PINS 1
    #define PORT_4_NUM_PINS 5
    #define PORT_5_NUM_PINS 4
    #define PORT_6_NUM_PINS 3

    extern const uint_fast16_t row_port[NUM_ROWS];
    extern const uint_fast16_t row_pins[NUM_ROWS];

    extern const uint8_t port_2_columns[PORT_2_NUM_PINS][2];
    extern const uint8_t port_3_columns[PORT_3_NUM_PINS][2];
    extern const uint8_t port_4_columns[PORT_4_NUM_PINS][2];
    extern const uint8_t port_5_columns[PORT_5_NUM_PINS][2];
    extern const uint8_t port_6_columns[PORT_6_NUM_PINS][2];
#endif // GLOBAL_H
