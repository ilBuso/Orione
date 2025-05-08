//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#pragma once

// Initializes the input device
// This function is responsible for setting up the input device,
// such as initializing necessary hardware or configuring software components
// for device interaction. The specifics will depend on the type of input device and system.
// Returns:
//    - 0 on success,
//    - Non-zero error code on failure.
int input_device_init();

// Cleans up the input device
// This function is used to release any resources or memory allocated during the
// input device's initialization or usage. It may handle device disconnection
// or cleanup of any buffers or system settings.
void input_device_cleanup();