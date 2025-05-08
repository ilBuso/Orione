//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#include <sys/_types/_null.h>

#include "profile_manager.h"
#include "rate_limiter.h"
#import "core/io/uart.h"
#import "core/io/input_device.h"
#import "core/logging.c"
#import "matrix/keycodes.h"
#import "matrix/matrix.h"

static ProfileManager profile_manager;
static RateLimiter rate_limiter;

void key_event_callback(int x, int y, bool pressed);

int main() {
    uart_init();
    input_device_init();

    profile_manager_init(&profile_manager, 100);

    rate_limiter_init(&rate_limiter, 50, key_event_callback);

    while (1) {
        const Message* msg = receive_message();

        if (msg != NULL)
        {
            // TODO: Figure out how we receive key press / release info
            handle_key_press(&rate_limiter, msg->x.data, msg->y.data);
        }
    }

    input_device_cleanup();
    uart_cleanup();
}

void key_event_callback(const int x, const int y, const bool pressed) {
    const int new_profile = keypress_has_triggered_profile(&profile_manager, x, y);
    if (new_profile != -1)
    {
        // todo: change matrix to new profile
    }

    const enum CrossPlatformKeyCode key_code = keyboard_matrix[x][y];
    input_device_send_key(key_code, pressed);
}