/**
 * @file init.c
 * @brief Hardware initialization implementation
 * 
 * Configures all GPIO pins for keyboard matrix (rows/columns), rotary encoder
 * (CLK, DT, SW), and status LEDs. Sets up appropriate pull-up/pull-down
 * resistors and pin directions.
 */

#include "init.h"

//--------------------------------------------------------------------+
// VARIABLES
//--------------------------------------------------------------------+
extern uint32_t last_encoder_time ;
extern uint32_t last_button_time;
extern uint8_t last_clk_state;

//--------------------------------------------------------------------+
// SUPPORT FUNCTIONS
//--------------------------------------------------------------------+

/**
 * @brief Initialize keyboard matrix row pins
 * 
 * Configures all row pins as outputs and sets them HIGH (inactive state).
 * Rows are driven HIGH one at a time during scanning to detect key presses.
 */
void init_row(void) {
    gpio_init(ROW_0);
    gpio_init(ROW_1);
    gpio_init(ROW_2);
    gpio_init(ROW_3);
    gpio_init(ROW_4);

    gpio_set_dir(ROW_0, GPIO_OUT);
    gpio_set_dir(ROW_1, GPIO_OUT);
    gpio_set_dir(ROW_2, GPIO_OUT);
    gpio_set_dir(ROW_3, GPIO_OUT);
    gpio_set_dir(ROW_4, GPIO_OUT);

    gpio_put(ROW_0, HIGH);
    gpio_put(ROW_1, HIGH);
    gpio_put(ROW_2, HIGH);
    gpio_put(ROW_3, HIGH);
    gpio_put(ROW_4, HIGH);
}

/**
 * @brief Initialize keyboard matrix column pins
 * 
 * Configures all column pins as inputs with pull-down resistors.
 * When a key is pressed, the column pin will read HIGH if its row is active.
 */
void init_column(void) {
    gpio_init(COLUMN_0);
    gpio_init(COLUMN_1);
    gpio_init(COLUMN_2);
    gpio_init(COLUMN_3);
    gpio_init(COLUMN_4);
    gpio_init(COLUMN_5);
    gpio_init(COLUMN_6);
    gpio_init(COLUMN_7);
    gpio_init(COLUMN_8);
    gpio_init(COLUMN_9);
    gpio_init(COLUMN_10);
    gpio_init(COLUMN_11);
    gpio_init(COLUMN_12);
    gpio_init(COLUMN_13);
    
    gpio_set_dir(COLUMN_0, GPIO_IN);
    gpio_set_dir(COLUMN_1, GPIO_IN);
    gpio_set_dir(COLUMN_2, GPIO_IN);
    gpio_set_dir(COLUMN_3, GPIO_IN);
    gpio_set_dir(COLUMN_4, GPIO_IN);
    gpio_set_dir(COLUMN_5, GPIO_IN);
    gpio_set_dir(COLUMN_6, GPIO_IN);
    gpio_set_dir(COLUMN_7, GPIO_IN);
    gpio_set_dir(COLUMN_8, GPIO_IN);
    gpio_set_dir(COLUMN_9, GPIO_IN);
    gpio_set_dir(COLUMN_10, GPIO_IN);
    gpio_set_dir(COLUMN_11, GPIO_IN);
    gpio_set_dir(COLUMN_12, GPIO_IN);
    gpio_set_dir(COLUMN_13, GPIO_IN);
    
    gpio_pull_down(COLUMN_0);
    gpio_pull_down(COLUMN_1);
    gpio_pull_down(COLUMN_2);
    gpio_pull_down(COLUMN_3);
    gpio_pull_down(COLUMN_4);
    gpio_pull_down(COLUMN_5);
    gpio_pull_down(COLUMN_6);
    gpio_pull_down(COLUMN_7);
    gpio_pull_down(COLUMN_8);
    gpio_pull_down(COLUMN_9);
    gpio_pull_down(COLUMN_10);
    gpio_pull_down(COLUMN_11);
    gpio_pull_down(COLUMN_12);
    gpio_pull_down(COLUMN_13);
}

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+

/**
 * @brief Initialize keyboard matrix GPIO pins
 * 
 * Calls initialization functions for both row and column pins
 * to set up the complete keyboard matrix.
 */
void init_keyboard_gpio(void) {
    init_row();
    init_column();
}

/**
 * @brief Initialize rotary encoder GPIO pins
 * 
 * Configures CLK, DT, and SW (button) pins as inputs with pull-up resistors.
 * Reads and stores the initial state of the CLK pin for rotation detection.
 */
void init_rotary_encoder_gpio(void) {
    gpio_init(ROTARY_CLK);
    gpio_set_dir(ROTARY_CLK, GPIO_IN);
    gpio_pull_up(ROTARY_CLK);
    
    gpio_init(ROTARY_DT);
    gpio_set_dir(ROTARY_DT, GPIO_IN);
    gpio_pull_up(ROTARY_DT);
    
    gpio_init(ROTARY_SW);
    gpio_set_dir(ROTARY_SW, GPIO_IN);
    gpio_pull_up(ROTARY_SW);
    
    // Read and store initial CLK state for edge detection
    last_clk_state = gpio_get(ROTARY_CLK);
}

/**
 * @brief Initialize keyboard matrix interrupts
 * 
 * Enables GPIO interrupts on all column pins for both rising and falling edges.
 * The first column (COLUMN_0) also registers the shared interrupt callback handler.
 */
void init_keyboard_interrupts(void) {
    gpio_set_irq_enabled_with_callback(COLUMN_0, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    
    gpio_set_irq_enabled(COLUMN_1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_2, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_3, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_4, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_5, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_6, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_7, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_8, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_9, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_10, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_11, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_12, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(COLUMN_13, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

/**
 * @brief Initialize rotary encoder interrupts
 * 
 * Enables GPIO interrupts for the rotary encoder:
 * - SW (button): Falling edge only (button press detection)
 * - CLK: Both edges (rotation detection)
 * The callback handler is shared with keyboard interrupts.
 */
void init_rotary_encoder_interrupts(void) {
    gpio_set_irq_enabled(ROTARY_SW, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(ROTARY_CLK, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}

/**
 * @brief Initialize Caps Lock LED
 * 
 * Configures the Caps Lock LED pin as an output and sets it to OFF (LOW).
 * The LED state is controlled by USB HID LED status reports from the host.
 */
void init_led(void) {
    gpio_init(CAPS_LOCK_LED);
    gpio_set_dir(CAPS_LOCK_LED, GPIO_OUT);
    gpio_put(CAPS_LOCK_LED, LOW);
}