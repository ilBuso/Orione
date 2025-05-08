//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#import "core/io/input_device.h"
#import "core/logging.c"

// TODO: Check libs needed (remove unnecessary)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <termios.h>

static int uinput_fd = -1;

int input_device_init()
{
    uinput_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinput_fd < 0)
    {
        LOG_ERROR("Error opening uinput device");
        return EXIT_FAILURE;
    }

    struct uinput_user_dev uidev = {};
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "MSP432 Virtual Keyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1234; // Vendor ID
    uidev.id.product = 0x5678; // Product ID
    uidev.id.version = 1;

    // Enable all the keys you want to simulate
    for (int i = 0; i < 256; i++)
    {
        ioctl(uinput_fd, UI_SET_KEYBIT, i);
    }
    ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinput_fd, UI_SET_EVBIT, EV_SYN);

    // Write the device configuration
    if (write(uinput_fd, &uidev, sizeof(uidev)) < 0)
    {
        LOG_ERROR("Error setting up uinput device");
        close(uinput_fd);
        return EXIT_FAILURE;
    }
    if (ioctl(uinput_fd, UI_DEV_CREATE) < 0)
    {
        LOG_ERROR("Error creating uinput device");
        close(uinput_fd);
        return EXIT_FAILURE;
    }

    LOG_DEBUG("Linux input device setup");

    return EXIT_SUCCESS;
}

void input_device_cleanup()
{
    if (uinput_fd >= 0)
    {
        ioctl(uinput_fd, UI_DEV_DESTROY);
        close(uinput_fd);
    }

    LOG_DEBUG("Linux input device cleaned up");
}
