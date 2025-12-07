#include <stdlib.h>
#include "../../message/message.h"
#include "interrupt.h"
#include "../debouncing/debouncing.h"

// 24 MHz DCO → 24.000.000 tick/s → 50 ms = 24e6 * 0.05 = 1.200.000 tick
#define RELEASE_DEBOUNCE_TICKS 3000000

void send_test_message(uint8_t row, uint8_t col, uint8_t pressed)
{
    Fragment* x_ptr = new_fragment(X, row);
    Fragment* y_ptr = new_fragment(Y, col);
    Fragment* info_ptr = new_fragment(INFO, pressed);


    // Check for memory allocation failures
    if (x_ptr == NULL || y_ptr == NULL || info_ptr == NULL) {
        if (x_ptr != NULL) free(x_ptr);
        if (y_ptr != NULL) free(y_ptr);
        if (info_ptr != NULL) free(info_ptr);
        return;
    }

    // Create message from fragments
    Message* msg = new_message(*x_ptr, *y_ptr, *info_ptr);
    if (msg == NULL) {
        free(x_ptr);
        free(y_ptr);
        free(info_ptr);
        return;
    }
    free(x_ptr);
    free(y_ptr);
    free(info_ptr);

    const uint32_t current_time = get_current_timestamp_ms();
    const uint32_t last_press_time = key_timestamps[row][col];

    if (pressed == 0)
    {
        if (send_message(msg)) {
            printf("release message sent\n");
        } else {
            printf("failed to send release message\n");
        }
    } else if(last_press_time == 0 || (current_time - last_press_time) > DEBOUNCE_THRESHOLD_MS){
        //Update timestamps
        key_timestamps[row][col] = current_time;

        // Send the message
        if (send_message(msg)) {
            printf("pressed message sent\n");
        } else {
            printf("failed to send pressed message\n");
        }
    }
}

// Timer32_1 interrupt handler (T32_INT2_IRQHandler for Timer32_1)
void T32_INT2_IRQHandler(void) {
    // Clear the underflow flag
    Timer32_clearInterruptFlag(TIMER32_1_BASE);

    // Stop the timer immediately to prevent continuous interrupts in periodic mode
    Timer32_haltTimer(TIMER32_1_BASE);

    // No new interrupt arrived for 50 ms:
    printf("released\n");
    send_test_message(2, 0, 0);
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
    int i = 0;
    for (i = 0; i < PORT_2_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_2_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P2, port_2_columns[i][0], 1);
            restart_timer();
            break;
        }
    }
}

void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    // Foreach column pin
    int i = 0;
    for (i = 0; i < PORT_3_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_3_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P3, port_3_columns[i][0], 1);
            restart_timer();
            break;
        }
    }
}

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    // Foreach column pin
    int i = 0;
    for (i = 0; i < PORT_4_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_4_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P4, port_4_columns[i][0], 1);
            restart_timer();
            break;
        }
    }
}

void PORT5_IRQHandler(void) {
    // TODO: remove after testing
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);
    
    send_test_message(2, 0, 1);
    restart_timer();
    /*
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // Foreach column pin
    int i = 0;
    for (i = 0; i < PORT_5_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_5_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P5, port_5_columns[i][0], 1);
            restart_timer();
            break;
        }
    }
    */
}

void PORT6_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    // Foreach column pin
    int i = 0;
    for (i = 0; i < PORT_6_NUM_PINS; i++) {
        // Check if is the origin of the interrupt
        if(status & port_6_columns[i][0]) {
            // Find triggering row
            scan_rows(GPIO_PORT_P6, port_6_columns[i][0], 1);
            restart_timer();
            break;
        }
    }
}
