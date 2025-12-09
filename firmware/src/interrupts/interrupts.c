/**
 * @file interrupts.c
 * @brief GPIO interrupt handler implementation
 *
 * Implements interrupt service routines for keyboard matrix key detection,
 * rotary encoder direction sensing, and Fn layer switching. For the keyboard
 * matrix, IRQs schedule a one-shot debounce alarm per column to sample
 * the stable pin state after a short delay (see `MATRIX_DEBOUNCE_TIME`).
 * The alarm callback (`column_debounce_alarm`) performs the actual row scan
 * and press/release handling. On release events, all rows are scanned to
 * correctly identify which key was released, handling multiple simultaneous
 * key presses on the same column.
 */

#include "interrupts.h"

//--------------------------------------------------------------------+

// Structure to track debounce state for each column
typedef struct {
    alarm_id_t alarm_id;
    bool last_stable_state;  // true = HIGH/pressed, false = LOW/released
} column_debounce_t;

typedef struct {
    alarm_id_t alarm_id;
    bool last_stable_state;  // true = pressed (LOW), false = released (HIGH)
} rotary_button_debounce_t;

static column_debounce_t column_debounce[14] = {0};
static rotary_button_debounce_t rotary_button_debounce = {0};

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
 * Any pending alarm for this column is cancelled to ensure we always process
 * the most recent edge transition, preventing missed key releases when
 * multiple keys on the same column are pressed in rapid succession.
 *
 * @param gpio GPIO pin number that triggered the interrupt
 * @param events Interrupt event flags (EDGE_RISE or EDGE_FALL)
 */
void keyboard_callback(uint gpio, uint32_t events) {
    uint8_t column = gpio - COLUMN_0;

    if (column >= 14) return;

    // Cancel any pending alarm for this column to ensure we process the latest event
    if (column_debounce[column].alarm_id != 0) {
        cancel_alarm(column_debounce[column].alarm_id);
        column_debounce[column].alarm_id = 0;
    }

    // Schedule new debounce alarm
    alarm_id_t aid = add_alarm_in_us(MATRIX_DEBOUNCE_TIME, column_debounce_alarm, (void*)(uintptr_t)column, true);
    if (aid >= 0) {
        column_debounce[column].alarm_id = aid;
    }
}

/**
 * @brief Alarm callback to process stabilized column state after debounce
 *
 * This alarm callback executes after `MATRIX_DEBOUNCE_TIME` microseconds and
 * reads the column pin once to decide whether the column is stably HIGH
 * (pressed) or LOW (released).
 * 
 * On press: Performs a row scan to identify which key was pressed and updates
 * kbd_state by calling keyboard_add_key.
 * 
 * On release: Scans ALL rows on this column to determine which specific key(s)
 * were released. This handles the case where multiple keys on the same column
 * are pressed/released in rapid succession, ensuring no releases are missed.
 *
 * @param id Alarm identifier for this callback invocation
 * @param user_data The column index (0..13) passed when scheduling the alarm
 * @return 0 (one-shot)
 */
static int64_t column_debounce_alarm(alarm_id_t id, void *user_data) {
    uint32_t col = (uintptr_t)user_data;
    if (col >= 14) return 0;

    uint gpio_pin = COLUMN_0 + col;
    bool current_state = (gpio_get(gpio_pin) == HIGH);
    
    column_debounce[col].alarm_id = 0;

    if (current_state) {
        // Key pressed - scan to find which row
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
            }
            keyboard_add_key(row, col);
        }
    } else {
        // Key released - need to check which key(s) on this column are still pressed
        // Build list of rows currently tracked for this column
        uint8_t pressed_rows[5];
        uint8_t pressed_count = 0;
        
        for (uint8_t i = 0; i < kbd_state.pressed_keys_count; i++) {
            if (kbd_state.pressed_keys[i][1] == col) {
                pressed_rows[pressed_count++] = kbd_state.pressed_keys[i][0];
            }
        }
        
        // Scan to see which keys are actually still pressed
        for (uint8_t r = 0; r < 5; r++) {
            gpio_put(ROW_0, (r == 0) ? HIGH : LOW);
            gpio_put(ROW_1, (r == 1) ? HIGH : LOW);
            gpio_put(ROW_2, (r == 2) ? HIGH : LOW);
            gpio_put(ROW_3, (r == 3) ? HIGH : LOW);
            gpio_put(ROW_4, (r == 4) ? HIGH : LOW);
            
            busy_wait_us(10);
            
            bool still_pressed = (gpio_get(gpio_pin) == HIGH);
            
            // Check if this row was tracked as pressed
            bool was_pressed = false;
            for (uint8_t i = 0; i < pressed_count; i++) {
                if (pressed_rows[i] == r) {
                    was_pressed = true;
                    break;
                }
            }
            
            // If it was pressed but isn't anymore, remove it
            if (was_pressed && !still_pressed) {
                if (r == FN_KEY_ROW && col == FN_KEY_COL) {
                    kbd_state.current_layer = 0;
                } else {
                    keyboard_remove_key(r, col);
                }
            }
        }
        
        // Restore idle state
        gpio_put(ROW_0, HIGH);
        gpio_put(ROW_1, HIGH);
        gpio_put(ROW_2, HIGH);
        gpio_put(ROW_3, HIGH);
        gpio_put(ROW_4, HIGH);
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
 * from mechanical bounce. Cancels any pending alarm to ensure the
 * most recent edge transition is processed. The alarm callback verifies
 * the stable button state and registers both press and release events.
 * 
 * @param gpio GPIO pin number (should be ROTARY_SW)
 * @param events Interrupt event flags
 */
void rotary_button_callback(uint gpio, uint32_t events) {
    // Cancel pending alarm if state is changing
    if (rotary_button_debounce.alarm_id != 0) {
        cancel_alarm(rotary_button_debounce.alarm_id);
        rotary_button_debounce.alarm_id = 0;
    }

    alarm_id_t aid = add_alarm_in_us(ENCODER_BTN_DEBOUNCE_TIME, rotary_button_debounce_alarm, NULL, true);
    if (aid >= 0) {
        rotary_button_debounce.alarm_id = aid;
    }
}

/**
 * @brief Alarm callback to process stabilized button state after debounce
 *
 * This alarm callback executes after `ENCODER_BTN_DEBOUNCE_TIME` microseconds
 * and reads the button pin to verify its stable state (LOW = pressed, HIGH = released).
 * Registers both press and release events by setting button_pressed accordingly
 * and setting has_event flag. State changes are tracked to avoid processing
 * duplicate events. The alarm is one-shot; its id is cleared before performing
 * state updates.
 *
 * @param id Alarm identifier for this callback invocation
 * @param user_data Unused for button debouncing
 * @return 0 (one-shot)
 */
static int64_t rotary_button_debounce_alarm(alarm_id_t id, void *user_data) {
    rotary_button_debounce.alarm_id = 0;

    // Button is active LOW (pressed = 0, released = 1)
    bool current_state = (gpio_get(ROTARY_SW) == 0);
    
    // Only process if state changed
    if (current_state == rotary_button_debounce.last_stable_state) {
        return 0;
    }
    
    rotary_button_debounce.last_stable_state = current_state;

    if (current_state) {
        // Button pressed (transition HIGH→LOW)
        rotary_state.button_pressed = true;
        rotary_state.has_event = true;
    } else {
        // Button released (transition LOW→HIGH)
        rotary_state.button_pressed = false;
        rotary_state.has_event = true;
    }

    return 0;
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