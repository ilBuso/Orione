//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#import "core/io/input_device.h"
#import "core/logging.c"

#include <_stdlib.h>

int input_device_init()
{
    // we don't create anything via HID for macOS
    LOG_DEBUG("macOS input device initialized");
    return EXIT_SUCCESS;
}

void input_device_cleanup()
{
    // nothing to do
    LOG_DEBUG("macOS input device cleaned up");
}