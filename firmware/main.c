/**
 * @file main.c
 * @brief Main entry point for Orione keyboard firmware
 * 
 * Handles USB device initialization, main loop execution, LED status blinking,
 * and HID report generation for both keyboard matrix and rotary encoder inputs.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "bsp/board_api.h"
#include "tusb.h"

#include "src/usb/usb_descriptors/usb_descriptors.h"
#include "src/usb/usb_callbacks/usb_callbacks.h"
#include "src/global.h"
#include "src/init/init.h"
#include "src/matrix/scan_rows/scan_rows.h"

//--------------------------------------------------------------------+

uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

keyboard_state_t kbd_state = {
    .has_new_key = false,
    .pressed_keys_count = 0,
    .current_layer = 0
};

//--------------------------------------------------------------------+

/**
 * @brief Blink onboard LED based on USB connection status
 * 
 * Provides visual feedback of the device state:
 * - Fast blink (250ms): Not mounted/connected
 * - Slow blink (1000ms): Mounted and ready
 * - Very slow blink (2500ms): Suspended
 * - Solid on: Caps Lock active
 * - Off: Blink disabled
 */
void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (!blink_interval_ms) return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms) return;
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state;
}

/**
 * @brief Process and send HID reports for keyboard and rotary encoder
 * 
 * Polls keyboard state and rotary encoder at 10ms intervals (required by TinyUSB).
 * Handles:
 * - Remote wakeup when suspended
 * - Keyboard key press/release reports
 * - Rotary encoder volume control (rotation) and mute (button press)
 */
void hid_task(void) {
    // Poll every 10ms -> required by TinyUSB
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    // check interval
    if (board_millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    // Remote wakeup if suspended
    if (tud_suspended()) {
        if (kbd_state.has_new_key) {
            tud_remote_wakeup();
            kbd_state.has_new_key = false;
        }
        return;
    } else {
        if (tud_hid_ready()) {
            // Handle keyboard input
            if (kbd_state.has_new_key) {
                // Clear flag
                kbd_state.has_new_key = false;
                // send report
                send_hid_report(REPORT_ID_KEYBOARD, 0);
            }

            // Handle rotary encoder input
            int8_t direction;
            bool button_pressed;
            
            // get state of rotary encoder
            rotary_encoder_get_state(&direction, &button_pressed);
            
            // check if rotary encoder is pressed or is turning
            if (direction != 0 || button_pressed) {
                if (!tud_hid_ready()) return;
                
                if (button_pressed) {
                    // mute/unmute
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE);
                    sleep_ms(10);
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0);
                } else if (direction > 0) {
                    // volume up
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
                    sleep_ms(10);
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0);
                } else if (direction < 0) {
                    // volume down
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
                    sleep_ms(10);
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0);
                }
            }
        }
    }
}

/**
 * @brief Initialize all hardware peripherals
 * 
 * Configures GPIO pins and interrupts for:
 * - Keyboard matrix (rows, columns, interrupts)
 * - Rotary encoder (CLK, DT, SW pins and interrupts)
 * - Status LEDs (Caps Lock indicator)
 */
void init(void) {
    // keyboard
    init_keyboard_gpio();
    init_keyboard_interrupts();
    
    // rotary encoder
    init_rotary_encoder_gpio();
    init_rotary_encoder_interrupts();

    // Caps-Lock led
    init_led();
}

//--------------------------------------------------------------------+

/**
 * @brief Main program entry point
 * 
 * Initializes the USB device stack and hardware peripherals, then enters
 * the main loop to continuously process USB tasks, update LED status,
 * and handle HID input/output.
 * 
 * @return Never returns (infinite loop)
 */
int main(void) {
    // init TinyUSB
    board_init();
    tud_init(BOARD_TUD_RHPORT);
    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    // init sys
    init();

    // loop
    while (1) {
        tud_task();
        led_blinking_task();
        hid_task();
    }
}