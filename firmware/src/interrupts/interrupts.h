#ifndef INTERRUPTS_H
#define INTERRUPTS_H

    #include "pico/stdlib.h"
    
    #include "../init/init.h"
    #include "../matrix/scan_rows/scan_rows.h"
    #include "../global.h"
    #include "../rotary_encoder/rotary_encoder.h"

    #define DEBOUNCE_TIME_US 50000

    void gpio_callback(uint gpio, uint32_t events);

#endif /* INTERRUPTS_H */