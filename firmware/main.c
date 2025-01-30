/**
 *  @file   main.c
 *  @brief  USB Keyboard main file
 *  @author Alessandro Busola
 *  @date   November 2024
 */

/// Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"

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

// Row and column bitmasks
const uint8_t row_bits[] = {BIT7, BIT6, BIT7, BIT0, BIT2, BIT6};
const uint8_t column_bits[] = {BIT5, BIT4, BIT7, BIT5, BIT4, BIT2, BIT0, BIT1, BIT5, BIT1, BIT3, BIT7, BIT6, BIT6, BIT4, BIT6, BIT7};

// Row and column ports
volatile uint8_t *row_ports[] = {&P5->OUT, &P1->OUT, &P1->OUT, &P5->OUT, &P5->OUT, &P3->OUT};
const volatile uint8_t *column_ports[] = {&P5->IN, &P5->IN, &P4->IN, &P4->IN, &P4->IN, &P4->IN, &P4->IN, &P6->IN, &P3->IN, &P5->IN, &P2->IN, &P6->IN, &P6->IN, &P5->IN, &P2->IN, &P2->IN, &P2->IN};

// Keymap
const int8_t keys[6][17] = {
    {1, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88, 99, 70, 119, -1},
    {41, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, -1, -1},
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 43, -1, -1, -1},
    {58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 28, -1, -1, -1, -1},
    {42, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, -1, -1, 103, -1, -1},
    {29, 125, 56, 57, -1, -1, -1, 100, 126, 97, -1, 105, 108, 106, -1, -1, -1}
};

/// Functions
/**
 *  @brief Init Ports Function
 *  This function initialize all the necessary GPIOs ports
 *  @return none
 */
void GPIO_init() {
    // ROW
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN7);

    // COLUMN
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P6, GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7);

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
void UART_init() {
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
    GPIO_init();
    // Initialize the USB APIs
    UART_init();
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

    // Main loop
    while (1) {
        int row = 0;
        for (; row < 6; row++) {
            // Set the current row LOW
            *row_ports[row] &= ~row_bits[row];

            // Set all other rows HIGH
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

                    uint8_t pressed_button = keys[row][column];
                    UART_transmitData(EUSCI_A2_BASE, pressed_button);

                    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
                }
            }

            // Reset the current row HIGH after scanning
            *row_ports[row] |= row_bits[row];
        }
    }
}
