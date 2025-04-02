#include "global.h"

const uint_fast16_t row_port[NUM_ROWS] = {
    GPIO_PORT_P5, GPIO_PORT_P1, GPIO_PORT_P1, GPIO_PORT_P5, GPIO_PORT_P5, GPIO_PORT_P3
};
const uint_fast16_t row_pins[NUM_ROWS] = {
    GPIO_PIN7, GPIO_PIN6, GPIO_PIN7, GPIO_PIN0, GPIO_PIN2, GPIO_PIN6
};

const uint8_t port_2_columns[PORT_2_NUM_PINS][2] = {
    {GPIO_PIN3, 10},
    {GPIO_PIN4, 14},
    {GPIO_PIN6, 15},
    {GPIO_PIN7, 16}
};

const uint8_t port_3_columns[PORT_3_NUM_PINS][2] = {
    {GPIO_PIN5, 8}
};

const uint8_t port_4_columns[PORT_4_NUM_PINS][2] = {
    {GPIO_PIN0, 6},
    {GPIO_PIN2, 5},
    {GPIO_PIN4, 4},
    {GPIO_PIN5, 3},
    {GPIO_PIN7, 2}
};

const uint8_t port_5_columns[PORT_5_NUM_PINS][2] = {
    {GPIO_PIN1, 9},
    {GPIO_PIN4, 1},
    {GPIO_PIN5, 0},
    {GPIO_PIN6, 13}
};

const uint8_t port_6_columns[PORT_6_NUM_PINS][2] = {
    {GPIO_PIN1, 7},
    {GPIO_PIN6, 12},
    {GPIO_PIN7, 11}
};