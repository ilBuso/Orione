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
void init_keyboard_gpio(void) {
    init_row();
    init_column();
}

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
    
    last_clk_state = gpio_get(ROTARY_CLK);
}

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

void init_rotary_encoder_interrupts(void) {
    // Set up interrupt for button (falling edge only)
    gpio_set_irq_enabled(ROTARY_SW, GPIO_IRQ_EDGE_FALL, true);
    // Set up interrupts for CLK (both edges for better detection)
    gpio_set_irq_enabled(ROTARY_CLK, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}