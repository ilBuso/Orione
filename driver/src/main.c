//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

//#include <sys/_types/_null.h>

#include "logging.h"
#include "profile/profile_manager.h"
#include "ratelimiter/rate_limiter.h"
#include "core/io/uart.h"
#include "core/io/input_device.h"
#include "matrix/keycodes.h"
#include "matrix/matrix.h"
#include "global/global.h"

static ProfileManager profile_manager;
static RateLimiter rate_limiter;

void key_event_callback(int x, int y, bool pressed);

int main()
{
    const int res = uart_init();
    if (res != 0) {
        perror("uart_init failed");
        return -1;
    }

    input_device_init();

    profile_manager_init(&profile_manager, 100);
    rate_limiter_init(&rate_limiter, 50, key_event_callback);

    while (1)
    {
        const Message* msg = receive_message();

        LOG_DEBUG("Unpacking message");
        // TODO: check rotary encoder key press instead of normal keyboard

        if (msg != NULL)
        {
            const bool pressed = check_pressed_flag(msg->info.data);

            LOG_DEBUG("Receive message for pressed: %d", pressed);

            if (pressed)
            {
                handle_key_press(&rate_limiter, msg->x.data, msg->y.data);
            }
            else
            {
                handle_key_release(&rate_limiter, msg->x.data, msg->y.data);
            }
        }
    }

    profile_manager_destroy(&profile_manager);
    rate_limiter_destroy(&rate_limiter);
    input_device_cleanup();
    uart_cleanup();
}

void key_event_callback(const int x, const int y, const bool pressed)
{
    LOG_DEBUG("Key event: pressed %d, x %d, y %d", pressed, x, y);

    enum CrossPlatformKeyCode keycode = keyboard_matrix[x][y];
    LOG_DEBUG("Cross platform keycode: %d", keycode);

    const int new_profile = keypress_has_triggered_profile(&profile_manager, keycode);
    if (new_profile != -1)
    {
        // todo: change matrix to new profile

        // reload the key_code if matrix changed
        keycode = keyboard_matrix[x][y];
    }

    input_device_send_key(keycode, pressed);
}
