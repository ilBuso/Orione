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


	/// RST
	// Set pin 4.1 as GPIO pins
	P4->SEL0 &= ~BIT1;
	P4->SEL1 &= ~BIT1;
	
	// Set pin 4.1 as output pin
	P4->DIR |= BIT1;

	// Default RTS high (ready to receive data)
	P4->OUT |= BIT1;


	/// CTS
	// Set pin 6.0 as GPIO pins
	P6->SEL0 &= ~BIT0;
	P6->SEL1 &= ~BIT0;

	// Set pin 6.0 as input pin
	P6->DIR &= ~BIT0;

	// Enable pull-up resistor and activate resistors for pin 6.0
	P6->OUT |= BIT0;
	P6->REN |= BIT0;


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
		
	}
}
