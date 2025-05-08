//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#pragma once

#import "core/matrix/keycodes.h"

/**
 * Initializes the input device.
 *
 * This function is responsible for setting up the input device,
 * such as initializing necessary hardware or configuring software components
 * for device interaction. The specifics will depend on the type of input device and system.
 *
 * @return 0 on success, non-zero error code on failure.
 */
int input_device_init();


/**
 * Sends a key event from the input device.
 *
 * This function is used to simulate a key press or release event from the input device.
 * It takes a key code and a boolean indicating whether the key is being pressed or released.
 *
 * @param key_code The key code representing the specific key event.
 * @param pressed  True if the key is being pressed, false if it is being released.
 */
void input_device_send_key(enum CrossPlatformKeyCode key_code, bool pressed);

/**
 * Cleans up the input device.
 *
 * This function is used to release any resources or memory allocated during the
 * input device's initialization or usage. It may handle device disconnection
 * or cleanup of any buffers or system settings.
 */
void input_device_cleanup();