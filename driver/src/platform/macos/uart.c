//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hid/IOHIDManager.h>

#include "core/io/uart.h"
#include "core/logging.c"

#ifndef MACOS_SERIAL_PORT
#define MACOS_SERIAL_PORT "/dev/cu.usbserial"
#endif

static int serial_fd = -1;
static IOHIDManagerRef hidManager = NULL;

int uart_init()
{
    const char* serialPort = MACOS_SERIAL_PORT;
    const int baudRate = B115200;

    // Open the UART device
    serial_fd = open(serialPort, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd < 0)
    {
        LOG_ERROR("Error opening serial port");
        return EXIT_FAILURE;
    }

    // Configure UART
    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0)
    {
        LOG_ERROR("Error getting terminal attributes");
        close(serial_fd);
        return EXIT_FAILURE;
    }
    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_cflag |= (CLOCAL | CREAD); // Enable reading
    tty.c_cflag &= ~(PARENB | CSTOPB); // No parity, 1 stop bit
    tty.c_lflag = 0; // Raw input
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0)
    {
        LOG_ERROR("Error setting terminal attributes");
        close(serial_fd);
        return EXIT_FAILURE;
    }

    LOG_DEBUG("macOS uart initialized");

    return EXIT_SUCCESS;
}

// Function to clean up resources
void uart_cleanup() {
    if (serial_fd >= 0) {
        close(serial_fd);
        serial_fd = -1;
    }

    LOG_DEBUG("macOS uart cleaned up");
}