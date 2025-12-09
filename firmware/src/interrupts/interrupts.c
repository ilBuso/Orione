/**
 * @file interrupts.c
 * @brief GPIO interrupt handler implementation
 *
 * Implements interrupt service routines for keyboard matrix key detection,
 * rotary encoder direction sensing, and Fn layer switching. For the keyboard
 * matrix, IRQs now schedule a one-shot debounce alarm per column to sample
 * the stable pin state after a short delay (see `MATRIX_DEBOUNCE_TIME`).
 * The alarm callback (`column_debounce_alarm`) performs the actual row scan
 * and press/release handling to avoid transient bounce-related mis-detections.
 */

#include "interrupts.h"

//--------------------------------------------------------------------+

static uint32_t last_interrupt_time[14] = {0};
// Alarm ids for per-column debounce timers
static alarm_id_t column_alarm_id[14] = {0};
// Alarm id for rotary button debounce timer
static alarm_id_t rotary_button_alarm_id = 0;
extern keyboard_state_t kbd_state;

extern uint32_t last_encoder_time;
extern uint32_t last_button_time;
extern uint8_t last_clk_state;
extern rotary_encoder_state_t rotary_state;

extern void keyboard_add_key(uint8_t row, uint8_t col);
extern void keyboard_remove_key(uint8_t row, uint8_t col);

// forward declaration for debounce alarm callback
static int64_t column_debounce_alarm(alarm_id_t id, void *user_data);
static int64_t rotary_button_debounce_alarm(alarm_id_t id, void *user_data);

//--------------------------------------------------------------------+

/**
 * @brief Keyboard matrix interrupt callback
 *
 * This callback is invoked directly from GPIO IRQs for column pins. Instead of
 * making an immediate press/release decision inside the IRQ (which is
 * vulnerable to switch bounce), the callback schedules a one-shot alarm to
 * run after `MATRIX_DEBOUNCE_TIME` microseconds. The alarm samples the
 * stabilized column level and performs the row scan and key add/remove
 * operations inside `column_debounce_alarm`.
 *
 * @param gpio GPIO pin number that triggered the interrupt
 * @param events Interrupt event flags (EDGE_RISE or EDGE_FALL)
 */
void keyboard_callback(uint gpio, uint32_t events) {
    // Defer stabilized state evaluation via alarm to avoid missed edges
    uint8_t column = gpio - COLUMN_0;

    if (column >= 14) return;

    // If a debounce alarm already scheduled for this column, ignore extra IRQs
    if (column_alarm_id[column] != 0) return;

    alarm_id_t aid = add_alarm_in_us(MATRIX_DEBOUNCE_TIME, column_debounce_alarm, (void*)(uintptr_t)column, true);
    if (aid >= 0) {
        column_alarm_id[column] = aid;
    }
}


/**
 * @brief Alarm callback to process stabilized column state after debounce
 *
 * This alarm callback executes after `MATRIX_DEBOUNCE_TIME` microseconds and
 * reads the column pin once to decide whether the column is stably HIGH
 * (pressed) or LOW (released). It performs a row scan when pressed and
 * updates `kbd_state` by calling `keyboard_add_key` or `keyboard_remove_key`.
 * The alarm is one-shot; its id is cleared in `column_alarm_id` before
 * performing state updates.
 *
 * @param id Alarm identifier for this callback invocation
 * @param user_data The column index (0..13) passed when scheduling the alarm
 * @return 0 (one-shot)
 */
static int64_t column_debounce_alarm(alarm_id_t id, void *user_data) {
    uint32_t col = (uintptr_t)user_data;
    if (col >= 14) return 0;

    // clear stored alarm id
    column_alarm_id[col] = 0;

    uint gpio_pin = COLUMN_0 + col;
    bool stable_high = (gpio_get(gpio_pin) == HIGH);

    if (stable_high) {
        uint8_t row = scan_rows(gpio_pin);

        // restore idle state of rows
        gpio_put(ROW_0, HIGH);
        gpio_put(ROW_1, HIGH);
        gpio_put(ROW_2, HIGH);
        gpio_put(ROW_3, HIGH);
        gpio_put(ROW_4, HIGH);

        if (row != 0xFF) {
            if (row == FN_KEY_ROW && col == FN_KEY_COL) {
                kbd_state.current_layer = 1;
            } else {
                keyboard_add_key(row, col);
            }
        }
    } else {
        bool fn_key_released = false;
        uint8_t row = 0xFF;
        for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
            if (kbd_state.pressed_keys[i][1] == col) {
                row = kbd_state.pressed_keys[i][0];
                break;
            }
        }

        if (row == 0xFF && kbd_state.current_layer == 1) {
            kbd_state.current_layer = 0;
            fn_key_released = true;
        }

        if (row == FN_KEY_ROW && col == FN_KEY_COL) {
            kbd_state.current_layer = 0;
            fn_key_released = true;
        }

        if (!fn_key_released && row != 0xFF) {
            keyboard_remove_key(row, col);
        }
    }

    return 0; // one-shot
}

/**
 * @brief Rotary encoder CLK pin interrupt callback
 * 
 * Detects rotation direction by reading CLK and DT pin states.
 * Uses falling edge detection with debouncing to determine clockwise
 * or counter-clockwise rotation.
 * 
 * @param gpio GPIO pin number (should be ROTARY_CLK)
 * @param events Interrupt event flags
 */
void rotary_clk_callback(uint gpio, uint32_t events) {
    uint32_t current_time = time_us_32();
    
    // Debounce
    if (current_time - last_encoder_time < ENCODER_CLK_DEBOUNCE_TIME) {
        return;
    }
    last_encoder_time = current_time;

    // Wait for signal to stabilize
    busy_wait_us(100); 
    
    // Read current state
    uint8_t clk_state = gpio_get(ROTARY_CLK);
    uint8_t dt_state = gpio_get(ROTARY_DT);
    
    // Detect direction on falling edge of CLK
    if (last_clk_state == 1 && clk_state == 0) {
        if (dt_state == 1) {
            // DT is HIGH during CLK fall = Clockwise rotation
            rotary_state.direction = 1;
        } else {
            // DT is LOW during CLK fall = Counter-clockwise rotation
            rotary_state.direction = -1;
        }
        rotary_state.has_event = true;
    }
    
    // store current state
    last_clk_state = clk_state;
}

/**
 * @brief Rotary encoder button interrupt callback
 * 
 * Schedules a one-shot alarm to sample the button state after 
 * `ENCODER_BTN_DEBOUNCE_TIME` microseconds, avoiding false triggers
 * from mechanical bounce. The alarm callback verifies the stable
 * button state before registering a press event.
 * 
 * @param gpio GPIO pin number (should be ROTARY_SW)
 * @param events Interrupt event flags
 */
void rotary_button_callback(uint gpio, uint32_t events) {
    // If a debounce alarm already scheduled for the button, ignore extra IRQs
    if (rotary_button_alarm_id != 0) return;

    alarm_id_t aid = add_alarm_in_us(ENCODER_BTN_DEBOUNCE_TIME, rotary_button_debounce_alarm, NULL, true);
    if (aid >= 0) {
        rotary_button_alarm_id = aid;
    }
}

/**
 * @brief Alarm callback to process stabilized button state after debounce
 *
 * This alarm callback executes after `ENCODER_BTN_DEBOUNCE_TIME` microseconds
 * and reads the button pin once to verify it's stably LOW (pressed).
 * Only registers a button press if the pin remains LOW after debounce.
 * The alarm is one-shot; its id is cleared before performing state updates.
 *
 * @param id Alarm identifier for this callback invocation
 * @param user_data Unused for button debouncing
 * @return 0 (one-shot)
 */
static int64_t rotary_button_debounce_alarm(alarm_id_t id, void *user_data) {
    // Clear stored alarm id
    rotary_button_alarm_id = 0;

    // Read stable button state (button is active LOW)
    bool stable_low = (gpio_get(ROTARY_SW) == 0);

    if (stable_low) {
        // Button is still pressed after debounce
        rotary_state.button_pressed = true;
        rotary_state.has_event = true;
    }

    return 0; // one-shot
}

//--------------------------------------------------------------------+

/**
 * @brief Main GPIO interrupt dispatcher
 * 
 * Routes GPIO interrupts to the appropriate handler based on which pin
 * triggered the interrupt. This single callback handles all keyboard matrix
 * columns, rotary encoder CLK, and rotary encoder button.
 * 
 * @param gpio GPIO pin number that triggered the interrupt
 * @param events Interrupt event flags (EDGE_RISE, EDGE_FALL, etc.)
 */
void gpio_callback(uint gpio, uint32_t events) {
    switch (gpio) {
        case ROTARY_CLK: {
            rotary_clk_callback(gpio, events);
            return;
        }
        break;
        
        case ROTARY_SW: {
            rotary_button_callback(gpio, events);
            return;
        }
        break;

        default: {
            // all other GPIOs are keyboard matrix columns
            keyboard_callback(gpio, events);
            return;
        }
        break;
    }
}