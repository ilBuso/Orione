/**
 *  @file   main.c
 *  @brief  USB Keyboard main file
 *  @author Alessandro Busola
 *  @date   November 2024
 */

/// Includes
#include "msp.h"
#include "stdio.h"

/// Functions
/**
 *  @brief Init Ports Function
 *  This function initialize all the necessary GPIOs ports
 *  @return none
 */
void init_ports() {
    /*
     * Keyboard Matrix
     */

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

    /*
     * UART
     */

    /// UART
    // Set pins 3.2 (RXD) and 3.3 (TXD) to their primary mode
    P3->SEL0 &= ~(BIT2 | BIT3);
    P3->SEL1 |= ~(BIT2 | BIT3);

    // Set pins 3.2 (RXD) as input
    P3->DIR &= ~BIT2;
    // Set pins 3.3 (TXD) as output
    P3->DIR |= BIT3;


    // To prevent a floating input and to reduce power consumption, unused I/O pins
    // should be configured as I/O function, output direction, and left unconnected
    // on the PC board. The value of the PxOUT bit is don't care,because the pin is
    // unconnected. Alternatively, the integrated pullup or pulldown resistor can
    // be enabled by setting the PxREN bit of the unused pin to prevent a floating
    // input.
}

/**
 *  @brief Init Function
 *  This function initializes the USB API Stack
 *  @return none
 */
void uart_init() {
    // TODO: Implement USB API initialization
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

/// Enums

/// Structs

/// Variables

/**
 *  @brief Main function
 *  Initializes the MCU and then jumps to a main loop routine checking the
 *  analog and digital keys
 *  @return none
 */
void main(void) {
    // Stop watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

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
                    // Button pressed at (row, col)
                    printf("Button pressed at row %d, column %d\n", row, column);
                }
            }

            // Reset the current row HIGH after scanning
            *row_ports[row] |= row_bits[row];
        }
    }
}