#include "interrupt.h"

// 24 MHz DCO → 24.000.000 tick/s → 50 ms = 24e6 * 0.05 = 1.200.000 tick
#define RELEASE_DEBOUNCE_TICKS 2000000

// Timer32_1 interrupt handler (T32_INT2_IRQHandler for Timer32_1)
void T32_INT2_IRQHandler(void) {
    // Clear the underflow flag
    Timer32_clearInterruptFlag(TIMER32_1_BASE);

    // Stop the timer immediately to prevent continuous interrupts in periodic mode
    Timer32_haltTimer(TIMER32_1_BASE);

    // No new interrupt arrived for 50 ms:
    printf("released\n");
}

void restart_timer(void) {
    // FERMA completamente il timer e cancella interrupt pendenti
    Timer32_haltTimer(TIMER32_1_BASE);
    Timer32_clearInterruptFlag(TIMER32_1_BASE);

    // REINIZIALIZZA il timer completamente in modalità periodic per controllare meglio
    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_enableInterrupt(TIMER32_1_BASE);

    // Imposta il nuovo conteggio e avvia
    Timer32_setCount(TIMER32_1_BASE, RELEASE_DEBOUNCE_TICKS);
    Timer32_startTimer(TIMER32_1_BASE, false);
}

void PORT2_IRQHandler(void) {
    // Save status and Clear flags
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    // Foreach column pin
    for (int i = 0; i < PORT_2_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_2_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P2, port_2_columns[i][0]);
            restart_timer();
            break;
        }
    }
}

void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    // Foreach column pin
    for (int i = 0; i < PORT_3_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_3_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P3, port_3_columns[i][0]);
            restart_timer();
            break;
        }
    }
}

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    // Foreach column pin
    for (int i = 0; i < PORT_4_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_4_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P4, port_4_columns[i][0]);
            restart_timer();
            break;
        }
    }
}

void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // Foreach column pin
    for (int i = 0; i < PORT_5_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_5_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P5, port_5_columns[i][0]);
            restart_timer();
            break;
        }
    }
}

void PORT6_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    // Foreach column pin
    for (int i = 0; i < PORT_6_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_6_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P6, port_6_columns[i][0]);
            restart_timer();
            break;
        }
    }
}
