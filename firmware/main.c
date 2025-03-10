#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"

#include "src/init/init.h"

void main(void) {
    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize all peripherals and settings
    init();

    // Enable global interrupts
    Interrupt_enableMaster();

    // Main loop
    while (1) {
        // Can put CPU into low power mode here if desired
        // PCM_gotoLPM0();
    }
}


/*#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"

#include <stdint.h>

/// Defines
#define NUM_ROWS 6
#define NUM_COLS 17

// Mapping arrays for rows
const uint_fast16_t row_port[] = {
    GPIO_PORT_P5, GPIO_PORT_P1, GPIO_PORT_P1, GPIO_PORT_P5, GPIO_PORT_P5, GPIO_PORT_P3
};
const uint_fast16_t row_pins[] = {
    GPIO_PIN7, GPIO_PIN6, GPIO_PIN7, GPIO_PIN0, GPIO_PIN2, GPIO_PIN6
};

// Column to index mapping for each port
// Structure: {pin, column_index}
const uint8_t port2_columns[][2] = {
    {GPIO_PIN3, 10},
    {GPIO_PIN4, 14},
    {GPIO_PIN6, 15},
    {GPIO_PIN7, 16}
};

const uint8_t port3_columns[][2] = {
    {GPIO_PIN5, 8}
};

const uint8_t port4_columns[][2] = {
    {GPIO_PIN0, 6},
    {GPIO_PIN2, 5},
    {GPIO_PIN4, 4},
    {GPIO_PIN5, 3},
    {GPIO_PIN7, 2}
};

const uint8_t port5_columns[][2] = {
    {GPIO_PIN1, 9},
    {GPIO_PIN4, 1},
    {GPIO_PIN5, 0},
    {GPIO_PIN6, 13}
};

const uint8_t port6_columns[][2] = {
    {GPIO_PIN1, 7},
    {GPIO_PIN6, 12},
    {GPIO_PIN7, 11}
};


// Key-map
const int8_t keys[NUM_ROWS][NUM_COLS] = {
    {1, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 87, 88, 99, 70, 119, -1},
    {41, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, -1, -1, -1},
    {15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 43, -1, -1, -1},
    {58, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 28, -1, -1, -1, -1},
    {42, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, -1, -1, 103, -1, -1},
    {29, 125, 56, 57, -1, -1, -1, 100, 126, 97, -1, 105, 108, 106, -1, -1, -1}
};

// Helper function to find column index
int8_t get_column_index(uint_fast16_t port, uint_fast16_t pin) {
    const uint8_t (*column_array)[2];
    uint8_t array_size = 0;

    switch(port) {
        case GPIO_PORT_P2:
            column_array = port2_columns;
            array_size = sizeof(port2_columns)/sizeof(port2_columns[0]);
            break;
        case GPIO_PORT_P3:
            column_array = port3_columns;
            array_size = sizeof(port3_columns)/sizeof(port3_columns[0]);
            break;
        case GPIO_PORT_P4:
            column_array = port4_columns;
            array_size = sizeof(port4_columns)/sizeof(port4_columns[0]);
            break;
        case GPIO_PORT_P5:
            column_array = port5_columns;
            array_size = sizeof(port5_columns)/sizeof(port5_columns[0]);
            break;
        case GPIO_PORT_P6:
            column_array = port6_columns;
            array_size = sizeof(port6_columns)/sizeof(port6_columns[0]);
            break;
        default:
            return -1;
    }

    int i;
    for (i = 0; i < array_size; i++) {
        if(column_array[i][0] == pin) {
            return column_array[i][1];
        }
    }

    return -1;
}

void scan_rows(uint_fast16_t port, uint_fast16_t pin) {
    // Get column index for the triggered pin
    int8_t column = get_column_index(port, pin);
    if (column < 0) return;

    // Set all rows low
    int i;
    for (i = 0; i < NUM_ROWS; i++) {
        GPIO_setOutputLowOnPin(row_port[i], row_pins[i]);
    }

    // Check each row
    int row;
    for (row = 0; row < NUM_ROWS; row++) {
        // Set current row high
        GPIO_setOutputHighOnPin(row_port[row], row_pins[row]);

        // Brief delay to stabilize
        //__delay_cycles(10);

        // Check if column is still high (key is at this row)
        if(GPIO_getInputPinValue(port, pin) == GPIO_INPUT_PIN_HIGH) {
            int8_t key_code = keys[row][column];

            // Toggle LED for visual feedback
            GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
            // Send key code
            UART_transmitData(EUSCI_A2_BASE, key_code);

            break;
        }

        // Set row back to low
        GPIO_setOutputLowOnPin(row_port[row], row_pins[row]);
    }

    // Restore all rows to high
    for (i = 0; i < NUM_ROWS; i++) {
        GPIO_setOutputHighOnPin(row_port[i], row_pins[i]);
    }
}

void PORT2_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < sizeof(port2_columns)/sizeof(port2_columns[0]); i++) {
        if(status & port2_columns[i][0]) {
            scan_rows(GPIO_PORT_P2, port2_columns[i][0]);
            break;
        }
    }
}

void PORT3_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P3);
    GPIO_clearInterruptFlag(GPIO_PORT_P3, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < sizeof(port3_columns)/sizeof(port3_columns[0]); i++) {
        if(status & port3_columns[i][0]) {
            scan_rows(GPIO_PORT_P3, port3_columns[i][0]);
            break;
        }
    }
}

void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    GPIO_clearInterruptFlag(GPIO_PORT_P4, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < sizeof(port4_columns)/sizeof(port4_columns[0]); i++) {
        if(status & port4_columns[i][0]) {
            scan_rows(GPIO_PORT_P4, port4_columns[i][0]);
            break;
        }
    }
}

void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < sizeof(port5_columns)/sizeof(port5_columns[0]); i++) {
        if(status & port5_columns[i][0]) {
            scan_rows(GPIO_PORT_P5, port5_columns[i][0]);
            break;
        }
    }
}

void PORT6_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P6);
    GPIO_clearInterruptFlag(GPIO_PORT_P6, status);

    // Debounce delay
    //__delay_cycles(50000);

    // Check each column pin
    int i;
    for (i = 0; i < sizeof(port6_columns)/sizeof(port6_columns[0]); i++) {
        if(status & port6_columns[i][0]) {
            scan_rows(GPIO_PORT_P6, port6_columns[i][0]);
            break;
        }
    }
}

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

void init(void) {
    // Initialize GPIO
    GPIO_init();
    // Initialize Interrupts
    interrupt_init();
    // Initialize UART
    UART_init();
}

void main(void) {
    // Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    // Initialize all peripherals and settings
    init();

    // Enable global interrupts
    Interrupt_enableMaster();

    // Main loop
    while (1) {
        // Can put CPU into low power mode here if desired
        // PCM_gotoLPM0();
    }
}
*/