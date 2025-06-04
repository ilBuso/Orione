#ifndef INTERRUPT_H
#define INTERRUPT_H

    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include <ti/devices/msp432p4xx/driverlib/Timer32.h>
    #include "msp.h"

    #include <stdio.h>
    #include <stdint.h>

    #include "../global/global.h"
    #include "../matrix/matrix.h"

    void PORT2_IRQHandler(void);
    void PORT3_IRQHandler(void);
    void PORT4_IRQHandler(void);
    void PORT5_IRQHandler(void);
    void PORT6_IRQHandler(void);

#endif // INTERRUPT_H
