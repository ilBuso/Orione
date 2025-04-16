#include "emulator/linux/emulator.h"
#include "message/message.h"
#include "parser/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <unistd.h>
#include <termios.h>

int main() {
    const char *serialPort = "/dev/ttyUSB0"; // Update to match your device node
    int baudRate = B115200;

    // Open the UART device
    int serialFd = open(serialPort, O_RDWR | O_NOCTTY | O_SYNC);
    if (serialFd < 0) {
        perror("Error opening serial port");
        return EXIT_FAILURE;
    }

    // Configure UART
    struct termios tty;
    if (tcgetattr(serialFd, &tty) != 0) {
        perror("Error getting terminal attributes");
        close(serialFd);
        return EXIT_FAILURE;
    }
    cfsetospeed(&tty, baudRate);
    cfsetispeed(&tty, baudRate);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_cflag |= (CLOCAL | CREAD);            // Enable reading
    tty.c_cflag &= ~(PARENB | CSTOPB);          // No parity, 1 stop bit
    tty.c_lflag = 0;                            // Raw input
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(serialFd, TCSANOW, &tty) != 0) {
        perror("Error setting terminal attributes");
        close(serialFd);
        return EXIT_FAILURE;
    }

    // Set up uinput
    int uinputFd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinputFd < 0) {
        perror("Error opening uinput device");
        close(serialFd);
        return EXIT_FAILURE;
    }

    struct uinput_user_dev uidev = {};
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "MSP432 Virtual Keyboard");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1234; // Vendor ID
    uidev.id.product = 0x5678; // Product ID
    uidev.id.version = 1;

    // Enable all the keys you want to simulate
    for (int i = 0; i < 256; i++) {
        ioctl(uinputFd, UI_SET_KEYBIT, i);
    }
    ioctl(uinputFd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinputFd, UI_SET_EVBIT, EV_SYN);

    // Write the device configuration
    if (write(uinputFd, &uidev, sizeof(uidev)) < 0) {
        perror("Error setting up uinput device");
        close(serialFd);
        close(uinputFd);
        return EXIT_FAILURE;
    }
    if (ioctl(uinputFd, UI_DEV_CREATE) < 0) {
        perror("Error creating uinput device");
        close(serialFd);
        close(uinputFd);
        return EXIT_FAILURE;
    }

    printf("Virtual keyboard initialized. Listening for keypresses...\n");

    // Parse toml
    const Parse* parse = parse_toml("src/matrix/linux.toml");
    if (!parse) {
        // error handling;
    }

    // Main loop
    while (1) {
        // Get message
        Message* msg = receive_message(serialFd);
        
        // If a message is received
        if (msg != NULL) {
            // emulate key
            if (!emulate_key(msg,uinputFd, parse)) {
                // error handling
            }
        }
    }

    // Cleanup
    ioctl(uinputFd, UI_DEV_DESTROY);
    close(uinputFd);
    close(serialFd);

    return EXIT_SUCCESS;
}