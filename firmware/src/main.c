/**
 * 	@file   main.c
 *  @brief  USB Keyboard main file
 *  @author Alessandro Busola
 *  @date   November 2024
 */

/// Include
#include "msp.h"

/// Functions
/**
 * 	@brief Init Clock Function
 *  This function initialize the clock for the usb and the rest of the system
 *	@return none
 */
void clock_init() {

}

/**
 * 	@brief Init Ports Function
 *  This function initialize all the necessary GPIOs ports
 *	@return none
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

	P3->SEL0 &= ~(BIT5);
	P3->SEL1 &= ~(BIT5);

	P4->SEL0 &= ~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7);
	P4->SEL1 &= ~(BIT0 | BIT2 | BIT4 | BIT5 | BIT7);

	P5->SEL0 &= ~(BIT1 | BIT4 | BIT5 | BIT6);
	P5->SEL1 &= ~(BIT1 | BIT4 | BIT5 | BIT6);

	P6->SEL0 &= ~(BIT1 | BIT6 | BIT7);
	P6->SEL1 &= ~(BIT1 | BIT6 | BIT7);

	// Set all the pins as input pins
	P2->DIR |= (BIT3 | BIT4 | BIT6 | BIT7);

	P3->DIR |= (BIT5);

	P4->DIR |= (BIT0 | BIT2 | BIT4 | BIT5 | BIT7);

	P5->DIR |= (BIT1 | BIT4 | BIT5 | BIT6);

	P6->DIR |= (BIT1 | BIT6 | BIT7);

	// Enable pull-up resistor and activate resistors for all pins
	P2->OUT |= (BIT3 | BIT4 | BIT6 | BIT7);
	P2->REN |= (BIT3 | BIT4 | BIT6 | BIT7);

	P3->OUT |= (BIT5);
	P3->REN |= (BIT5);

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
	P3->DIR &= ~BIT2
	// Set pins 3.3 (TXD) as output
	P3->DIR |= BIT3


	// To prevent a floating input and to reduce power consumption, unused I/O pins
	// should be configured as I/O function, output direction, and left unconnected
	// on the PC board. The value of the PxOUT bit is don't care,because the pin is
	// unconnected. Alternatively, the integrated pullup or pulldown resistor can
	// be enabled by setting the PxREN bit of the unused pin to prevent a floating 
	// input.
}

/**
 * 	@brief Init Function
 *  This function initialize the USB API Stack
 *	@return none
 */
void usb_api_init() {

}

/// Enums

/// Structs

/// Variables

/**
 * 	@brief Init Function
 *  This function calls all the necessary initialization functions
 *	@return none
 */
void init(void) {
	// initialize the clock
	clock_init();
	// initialize the ports
	init_ports();
	// init the USB APIs
	usb_api_init();
}

/**
 * 	@brief Main function
 *  Initializes the MCU and then jumps to a main loop routine checking the
 *  analog and digital keys
 *	@return none
 */
void main(void)
{
	// stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// call init functions
	init();

	// main loop
	while(1) {
		// Check each row
		// Array of row and column bitmasks for easy access
		const uint8_t rowBits[] = {BIT7, BIT6, BIT7, BIT0, BIT2, BIT6}; // Rows
		const uint8_t colBits[] = {
			BIT5, BIT4, BIT7, BIT5, BIT4, BIT2, BIT0, // Columns 0-6
			BIT1, BIT5, BIT1, BIT3, BIT7, BIT6, BIT6, BIT4, BIT6, BIT7 // Columns 7-16
		};

		// Corresponding ports for rows and columns
		GPIO_Type *rowPorts[] = {P5, P1, P1, P5, P5, P3};
		GPIO_Type *colPorts[] = {
			P5, P5, P4, P4, P4, P4, P4, // Columns 0-6
			P6, P3, P5, P2, P6, P6, P5, P2, P2, P2 // Columns 7-16
		};

		// Matrix scanning loop
		for (int row = 0; row < 6; row++) {
			// Set the current row low
			rowPorts[row]->OUT &= ~rowBits[row];

			// Set all other rows high
			for (int otherRow = 0; otherRow < 6; otherRow++) {
				if (otherRow != row) {
					rowPorts[otherRow]->OUT |= rowBits[otherRow];
				}
			}

			// Read all columns
			for (int col = 0; col < 17; col++) {
				if ((colPorts[col]->IN & colBits[col]) == 0) {
					// Button pressed at (row, col)
					printf("Row %d - Column %d\n", row, col);
				}
			}

			// Reset the current row high after scanning
			rowPorts[row]->OUT |= rowBits[row];
		}
	}
}