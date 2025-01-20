/**
 *  @file   main.c
 *  @brief  USB Keyboard main file
 *  @author Alessandro Busola
 *  @date   November 2024
 */

/// Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"

#include "stdio.h"
#include <stdint.h>
#include <stdbool.h>

/// Variables
const eUSCI_UART_ConfigV1 uartConfig = {
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // SMCLK Clock Source
    13,                                             // BRDIV = 13
    0,                                              // UCxBRF = 0
    37,                                             // UCxBRS = 37
    EUSCI_A_UART_NO_PARITY,                         // No Parity
    EUSCI_A_UART_LSB_FIRST,                         // LSB First
    EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
    EUSCI_A_UART_MODE,                              // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
    EUSCI_A_UART_8_BIT_LEN                          // 8 bit data length
};

/// Enums

/// Structs
typedef struct {
    uint8_t row;
    uint8_t column;
} KeyPair;

/// Functions
/**
 *  @brief Init Ports Function
 *  This function initialize all the necessary GPIOs ports
 *  @return none
 */
void init_ports() {
    /// ROW
    // Set all the pins as GPIO pins
    P1->SEL0 &= ~(BIT6 | BIT7);
    P1->SEL1 &= ~(BIT6 | BIT7);

    P3->SEL0 &= ~(BIT6);
    P3->SEL1 &= ~(BIT6);

    P5->SEL0 &= ~(BIT0 | BIT2 | BIT7);
    P5->SEL1 &= ~(BIT0 | BIT2 | BIT7);

    // Set all the pins as output pins
    P1->DIR |= (BIT6 | BIT7);

    P3->DIR |= (BIT6);

    P5->DIR |= (BIT0 | BIT2 | BIT7);


    /// COLUMN
    // Set all the pins as GPIO pins
    P2->SEL0 &= ~(BIT3 | BIT4 | BIT6 | BIT7);
    P2->SEL1 &= ~(BIT3 | BIT4 | BIT6 | BIT7);

    P3->SEL0 &= ~BIT5;
    P3->SEL1 &= ~BIT5;

    P4->SEL0 &= ~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7);
    P4->SEL1 &= ~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7);

    P5->SEL0 &= ~(BIT1 | BIT4 | BIT5 | BIT6);
    P5->SEL1 &= ~(BIT1 | BIT4 | BIT5 | BIT6);

    P6->SEL0 &= ~(BIT1 | BIT6 | BIT7);
    P6->SEL1 &= ~(BIT1 | BIT6 | BIT7);

    // Set all the pins as input pins
    P2->DIR &= ~(BIT3 | BIT4 | BIT6 | BIT7);

    P3->DIR &= ~BIT5;

    P4->DIR &= ~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7);

    P5->DIR &= ~(BIT1 | BIT4 | BIT5 | BIT6);

    P6->DIR &= ~(BIT1 | BIT6 | BIT7);

    // Enable pull-up resistor and activate resistors for all pins
    P2->OUT |= (BIT3 | BIT4 | BIT6 | BIT7);
    P2->REN |= (BIT3 | BIT4 | BIT6 | BIT7);

    P3->OUT |= BIT5;
    P3->REN |= BIT5;

    P4->OUT |= (BIT0 | BIT2 | BIT4 | BIT5 | BIT7);
    P4->REN |= (BIT0 | BIT2 | BIT4 | BIT5 | BIT7);

    P5->OUT |= (BIT1 | BIT4 | BIT5 | BIT6);
    P5->REN |= (BIT1 | BIT4 | BIT5 | BIT6);

    P6->OUT |= (BIT1 | BIT6 | BIT7);
    P6->REN |= (BIT1 | BIT6 | BIT7);

    // To prevent a floating input and to reduce power consumption, unused I/O pins
    // should be configured as I/O function, output direction, and left unconnected
    // on the PC board. The value of the PxOUT bit is don't care,because the pin is
    // unconnected. Alternatively, the integrated pullup or pulldown resistor can
    // be enabled by setting the PxREN bit of the unused pin to prevent a floating
    // input.
}

/**
 *  @brief Init Function
 *  This function initializes the UART
 *  @return none
 */
void uart_init() {
    // Setting P3.2 and P3.3 to their primary function (UART)
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Settign P1.0 as output (LED)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Setting DCO to 24MHz (upping Vcore) -> CPU operates at 24 MHz
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    // Configuring UART Module
    UART_initModule(EUSCI_A2_BASE, &uartConfig);

    // Enable UART module
    UART_enableModule(EUSCI_A2_BASE);
}

/**
 *  @brief Init Function
 *  This function calls all the necessary initialization functions
 *  @return none
 */
void init(void) {
    // Initialize the ports
    init_ports();
    // Initialize the USB APIs
    uart_init();
}

/**
 *  @brief Main function
 *  Initializes the MCU and then jumps to a main loop routine checking the
 *  analog and digital keys
 *  @return none
 */
void main(void) {
    // Stop watchdog timer
    WDT_A_holdTimer();

    // Call init functions
    init();

    // Row and column bitmasks
    const uint8_t row_bits[] = {BIT7, BIT6, BIT7, BIT0, BIT2, BIT6};
    const uint8_t column_bits[] = {BIT5, BIT4, BIT7, BIT5, BIT4, BIT2, BIT0, BIT1, BIT5, BIT1, BIT3, BIT7, BIT6, BIT6, BIT4, BIT6, BIT7};

    // Row ports
    volatile uint8_t *row_ports[] = {&P5->OUT, &P1->OUT, &P1->OUT, &P5->OUT, &P5->OUT, &P3->OUT};

    // Column ports
    const volatile uint8_t *column_ports[] = {&P5->IN, &P5->IN, &P4->IN, &P4->IN, &P4->IN, &P4->IN, &P4->IN, &P6->IN, &P3->IN, &P5->IN, &P2->IN, &P6->IN, &P6->IN, &P5->IN, &P2->IN, &P2->IN, &P2->IN};

    // Main loop
    while (1) {
        int row = 0;
        for (; row < 6; row++) {
            // Set the current row LOW
            *row_ports[row] &= ~row_bits[row];

            // Set all other rows HIGHù
            int other_rows = 0;
            for (; other_rows < 6; other_rows++) {
                if (other_rows != row) {
                    *row_ports[other_rows] |= row_bits[other_rows];
                }
            }

            // Read all columns
            int column = 0;
            for (; column < 17; column++) {
                if ((*column_ports[column] & column_bits[column]) == 0) {
                    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);



                    // Button pressed at (row, col)
                    printf("Button pressed at row %d, column %d\n", row, column);
                    UART_transmitData(EUSCI_A2_BASE, *pressed_button);
                    UART_transmitData(EUSCI_A2_BASE, '\n');

                    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                }
            }

            // Reset the current row HIGH after scanning
            *row_ports[row] |= row_bits[row];
        }
    }
}