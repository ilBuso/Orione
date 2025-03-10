#ifndef INIT_H
#define INIT_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    // Initialize GPIO pins
    void GPIO_init(void);

    // Initialize interrupts
    void interrupt_init(void);

    // Initialize UART
    void UART_init(void);

    // Main initialization function
    void init(void);

#endif // INIT_H
