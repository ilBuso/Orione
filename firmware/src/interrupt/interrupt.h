/**
 * @file interrupt.h
 * @brief Interrupt handlers for keyboard matrix scanning
 * @author Alessandro Busola
 * @date   March 2024
 *
 * This file declares the interrupt handlers for the GPIO ports used in
 * the keyboard matrix, as well as the function to initialize interrupts.
 */


#ifndef INTERRUPT_H
#define INTERRUPT_H

    // Includes
    // TI
    #include <ti/devices/msp432p4xx/driverlib/driverlib.h>
    #include "msp.h"

    // Libraries
    #include <stdint.h>

    // Header files
    #include "../global/global.h"

    // Port interrupt handlers
    void PORT2_IRQHandler(void);
    void PORT3_IRQHandler(void);
    void PORT4_IRQHandler(void);
    void PORT5_IRQHandler(void);
    void PORT6_IRQHandler(void);

#endif // INTERRUPT_H
