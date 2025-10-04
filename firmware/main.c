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
// VARIABLES
//--------------------------------------------------------------------+
uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
keyboard_state_t kbd_state = {0};

//--------------------------------------------------------------------+
// FUNCTIONS
//--------------------------------------------------------------------+

void led_blinking_task(void) {
    static uint32_t start_ms = 0;
    static bool led_state = false;

    // blink is disabled
    if (!blink_interval_ms) return;

    // Blink every interval ms
    if (board_millis() - start_ms < blink_interval_ms) return; // not enough time
    start_ms += blink_interval_ms;

    board_led_write(led_state);
    led_state = 1 - led_state; // toggle
}

void hid_task(void) {
    // Poll every 10ms
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms) return;
    start_ms += interval_ms;

    // Remote wakeup if suspended (check once for both keyboard and encoder)
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
                kbd_state.has_new_key = false;  // Clear flag immediately
                send_hid_report(REPORT_ID_KEYBOARD, 0);  // consumer_code is ignored for keyboard
            }

            // Handle rotary encoder input
            int8_t direction;
            bool button_pressed;
            
            rotary_encoder_get_state(&direction, &button_pressed);
            
            if (direction != 0 || button_pressed) {
                if (!tud_hid_ready()) return;
                
                if (button_pressed) {
                    // Mute/Unmute
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_MUTE);
                    sleep_ms(10); // Small delay
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0); // Release
                } else if (direction > 0) {
                    // Volume up
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_INCREMENT);
                    sleep_ms(10);
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0); // Release
                } else if (direction < 0) {
                    // Volume down
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, HID_USAGE_CONSUMER_VOLUME_DECREMENT);
                    sleep_ms(10);
                    send_hid_report(REPORT_ID_CONSUMER_CONTROL, 0); // Release
                }
            }
        }
    }
}

void init(void) {
    init_keyboard_gpio();
    init_keyboard_interrupts();
    init_rotary_encoder_gpio();
    init_rotary_encoder_interrupts();
}

//--------------------------------------------------------------------+
// MAIN
//--------------------------------------------------------------------+

int main(void) {
    board_init();
    tud_init(BOARD_TUD_RHPORT);
    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    init();

    while (1) {
        tud_task();
        led_blinking_task();
        hid_task();
    }
}