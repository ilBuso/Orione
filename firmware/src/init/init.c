/**
 * @file init.c
 * @brief Implementation of initialization functions
 * @author Alessandro Busola
 * @date   March 2024
 * 
 * This file implements the initialization functions declared in init.h.
*/

#include "init.h"
#include "global.h"

void GPIO_init(void) {
    /// ROWs
    // Set corresponding pins to output
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN7);

    // Set them initially as high
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6 | GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN7);

    /// COLUMNs
    // Set corresponding pins to input with pull-down resistors
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P6, GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7);
    
    // To prevent a floating input and to reduce power consumption, unused I/O pins
	// should be configured as I/O function, output direction, and left unconnected
	// on the PC board. The value of the PxOUT bit is don't care,because the pin is
	// unconnected. Alternatively, the integrated pullup or pulldown resistor can
	// be enabled by setting the PxREN bit of the unused pin to prevent a floating 
	// input.
}

void UART_init(void) {
    // Configure UART settings
    eUSCI_UART_ConfigV1 uartConfig = {
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

    // Configure UART pins
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    // Settign P1.0 as output (LED)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Setting DCO to 24MHz (upping Vcore) -> CPU operates at 24 MHz
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);

    // Configure and Enable UART Module
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    UART_enableModule(EUSCI_A2_BASE);
}

void interrupt_init(void) {
    // Configure LOW to HIGH transition interrupts for COLUMNs
    GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN6 | GPIO_PIN7, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P3, GPIO_PIN5, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P6, GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7, GPIO_LOW_TO_HIGH_TRANSITION);

    // Clear any pending interrupts
    GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7);

    // Enable interrupts on the columns
    GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN6 | GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN1 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P6, GPIO_PIN1 | GPIO_PIN6 | GPIO_PIN7);

    // Enable port interrupts in NVIC
    Interrupt_enableInterrupt(INT_PORT2);
    Interrupt_enableInterrupt(INT_PORT3);
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT6);
}

void rotary_encoder_init(void) {
    // Configure ChannelA and ChannelB as input with pull-up resistor
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1 | GPIO_PIN3);

    //Initialise the current state of Channel A
    last_channel_a_state = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN3);

    // Enable Interrupt on ChannelA
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, GPIO_PIN3, GPIO_BOTH_EDGES);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN3);

    // Configure one end on SW1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);

    // Enable Interrupt on SW1
    GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN5);

    //Enable PORT1 interrupt in NVIC
    Interrupt_enableInterrupt(INT_PORT1);
}

void init(void) {
    // Initialize GPIO
    GPIO_init();
    // Initialize Interrupts
    interrupt_init();
    // Initialize UART
    UART_init();

    rotary_encoder_init();

    UART_transmitData(EUSCI_A2_BASE, volume);
}
