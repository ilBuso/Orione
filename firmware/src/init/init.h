#ifndef INIT_H
#define INIT_H

    #include "pico/stdlib.h"

    #include "../matrix/matrix.h"
    #include "../rotary_encoder/rotary_encoder.h"
    #include "../interrupts/interrupts.h"

    #define GPIO_OUT true
    #define GPIO_IN false

    void init_keyboard_gpio(void);
    void init_rotary_encoder_gpio(void);
    void init_keyboard_interrupts(void);
    void init_rotary_encoder_interrupts(void);

#endif /* INIT_H */