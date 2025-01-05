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
	// PIN 1
	P1->SEL0 &= ~(BIT6 | BIT7);
	P1->SEL1 &= ~(BIT6 | BIT7);	

	// PIN 2
	P2->SEL0 &= ~(BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
	P2->SEL1 &= ~(BIT3 | BIT4 | BIT5 | BIT6 | BIT7);	

	// PIN 3
	P3->SEL0 &= ~(BIT0 | BIT2 | BIT3 | BIT5 | BIT6);
	P3->SEL1 &= ~(BIT0 | BIT2 | BIT3 | BIT5 | BIT6);

	// PIN 4
	P4->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);
	P4->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT7);

	// PIN 5
	P5->SEL0 &= ~(BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6 | BIT7);
	P5->SEL1 &= ~(BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6 | BIT7);

	// PIN 6
	P6->SEL0 &= ~(BIT1 | BIT6 | BIT7);
	P6->SEL1 &= ~(BIT1 | BIT6 | BIT7);
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

/// Global variables

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
		// check USB events
	}
}
