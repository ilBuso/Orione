//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#include <sys/_types/_null.h>

#include "profile_manager.h"
#include "rate_limiter.h"
#import "core/io/uart.h"
#import "core/io/input_device.h"
#import "core/logging.c"

static ProfileManager profile_manager;
static RateLimiter rate_limiter;

void key_event_callback(int x, int y, bool is_press);

int main() {
    uart_init();
    input_device_init();

    profile_manager_init(&profile_manager, 100);

    rate_limiter_init(&rate_limiter, 50, key_event_callback);

    while (1) {
        Message* msg = receive_message();

        if (msg != NULL)
        {
            // TODO: Figure out how we receive key press / release info
            handle_key_press(&rate_limiter, msg->x.data, msg->y.data);
        }
    }

    input_device_cleanup();
    uart_cleanup();
}

void key_event_callback(const int x, const int y, const bool is_press) {
    if (is_press) {
        LOG_DEBUG("Key pressed at (%d, %d)\n", x, y);
    } else {
        LOG_DEBUG("Key released at (%d, %d)\n", x, y);
    }
}