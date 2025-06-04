#ifndef MATRIX_H
#define MATRIX_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    #include <stdint.h>
    #include <stdio.h>

    #include "../global/global.h"

    int8_t get_column_index(uint_fast16_t port, uint_fast16_t pin);
    void scan_rows(uint_fast16_t port, uint_fast16_t pin, uint8_t pressed);

#endif // MATRIX_H
 
