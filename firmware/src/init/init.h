#ifndef INIT_H
#define INIT_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include <ti/devices/msp432p4xx/driverlib/Timer32.h>
    #include "msp.h"
    
    void GPIO_init(void);
    void interrupt_init(void);
    void UART_init(void);
    void init(void);

#endif // INIT_H
