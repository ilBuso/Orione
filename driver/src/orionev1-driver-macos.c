#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <stdbool.h>
#include <ApplicationServices/ApplicationServices.h>

// Global variables
int uart_fd = -1;

// Function prototypes
int setup_uart(const char *port, int baud);
CGKeyCode ascii_to_keycode(char c);
void simulate_key_press(CGKeyCode keycode);
void cleanup(void);

int main(int argc, char *argv[]) {
    // Extract command line arguments
    const char *uart_device = "/dev/cu.usbserial-FTDF92XE";
    int baud_rate = 115200;

    // Setup UART
    uart_fd = setup_uart(uart_device, baud_rate);
    if (uart_fd < 0) {
        fprintf(stderr, "Failed to open UART device %s\n", uart_device);
        return 1;
    }
    printf("UART device %s opened successfully at %d baud\n", uart_device, baud_rate);

    // Register cleanup function
    atexit(cleanup);

    // Main loop to read UART and send keyboard events
    printf("Starting UART to keyboard simulator. Press Ctrl+C to exit.\n");
    // char buffer[256];

    unsigned char buffer[1];

    while (1) {
        ssize_t bytes_read = read(uart_fd, buffer, sizeof(buffer));
        if (bytes_read > 0) {
            printf("Received: %c %d\n", buffer[0], buffer[0]);

            // Process each character
            CGKeyCode keycode = ascii_to_keycode(buffer[0]);
            if (keycode != (CGKeyCode)-1) {
                simulate_key_press(keycode);
                usleep(10000);  // 10ms delay between key presses
            }
        } else {
            // Small delay to prevent CPU hogging
            usleep(10000);
        }
    }

    return 0;
}

int setup_uart(const char *port, int baud) {
    int fd;
    struct termios tty;
    speed_t baud_const;

    // Open the serial port
    // fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    // Get current attributes
    if (tcgetattr(fd, &tty) < 0) {
        perror("tcgetattr");
        close(fd);
        return -1;
    }

    // Set baud rate
    switch (baud) {
        case 9600:   baud_const = B9600;   break;
        case 19200:  baud_const = B19200;  break;
        case 38400:  baud_const = B38400;  break;
        case 57600:  baud_const = B57600;  break;
        case 115200: baud_const = B115200; break;
        default:
            fprintf(stderr, "Unsupported baud rate: %d\n", baud);
            close(fd);
            return -1;
    }

    // Apply the Linux UART settings you provided
    cfsetospeed(&tty, baud_const);
    cfsetispeed(&tty, baud_const);
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit chars
    tty.c_cflag |= (CLOCAL | CREAD);            // Enable reading
    tty.c_cflag &= ~(PARENB | CSTOPB);          // No parity, 1 stop bit
    tty.c_lflag = 0;                            // Raw input
    tty.c_cc[VMIN] = 1;                         // Wait for at least 1 character
    tty.c_cc[VTIME] = 1;                        // With a timeout of 0.1 seconds

    // Apply new settings
    if (tcsetattr(fd, TCSANOW, &tty) < 0) {
        perror("tcsetattr");
        close(fd);
        return -1;
    }

    // Clear the line
    tcflush(fd, TCIOFLUSH);

    return fd;
}

CGKeyCode ascii_to_keycode(char c) {
    // Map from ASCII to macOS virtual key codes
    // These are the most common codes, may need adjustment for your specific needs
    switch (c) {
        // Letters
        case 'a': case 'A': return 0;
        case 's': case 'S': return 1;
        case 'd': case 'D': return 2;
        case 'f': case 'F': return 3;
        case 'h': case 'H': return 4;
        case 'g': case 'G': return 5;
        case 'z': case 'Z': return 6;
        case 'x': case 'X': return 7;
        case 'c': case 'C': return 8;
        case 'v': case 'V': return 9;
        case 'b': case 'B': return 11;
        case 'q': case 'Q': return 12;
        case 'w': case 'W': return 13;
        case 'e': case 'E': return 14;
        case 'r': case 'R': return 15;
        case 'y': case 'Y': return 16;
        case 't': case 'T': return 17;
        case '1': case '!': return 18;
        case '2': case '@': return 19;
        case '3': case '#': return 20;
        case '4': case '$': return 21;
        case '6': case '^': return 22;
        case '5': case '%': return 23;
        case '=': case '+': return 24;
        case '9': case '(': return 25;
        case '7': case '&': return 26;
        case '-': case '_': return 27;
        case '8': case '*': return 28;
        case '0': case ')': return 29;
        case ']': case '}': return 30;
        case 'o': case 'O': return 31;
        case 'u': case 'U': return 32;
        case '[': case '{': return 33;
        case 'i': case 'I': return 34;
        case 'p': case 'P': return 35;
        case '\r': case '\n': return 36;  // Return/Enter
        case 'l': case 'L': return 37;
        case 'j': case 'J': return 38;
        case '\'': case '"': return 39;
        case 'k': case 'K': return 40;
        case ';': case ':': return 41;
        case '\\': case '|': return 42;
        case ',': case '<': return 43;
        case '/': case '?': return 44;
        case 'n': case 'N': return 45;
        case 'm': case 'M': return 46;
        case '.': case '>': return 47;
        case '\t': return 48;  // Tab
        case ' ': return 49;   // Space
        case '`': case '~': return 50;

        // Function keys
        case '\b': return 51;  // Backspace/Delete

        default: return (CGKeyCode)-1;  // Unknown character
    }
}

void simulate_key_press(CGKeyCode keycode) {
    // Create a keyboard event source
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    if (source == NULL) {
        fprintf(stderr, "Failed to create event source\n");
        return;
    }

    // Create key down event
    CGEventRef keyDown = CGEventCreateKeyboardEvent(source, keycode, true);

    // Create key up event
    CGEventRef keyUp = CGEventCreateKeyboardEvent(source, keycode, false);

    // Post events to the system
    if (keyDown && keyUp) {
        // Handle uppercase letters and special characters
        // For uppercase letters, we need to simulate a shift key press
        char c = ' ';
        if (keycode >= 0 && keycode <= 50) {
            c = "asdfhgzxcvbqwerytl1234sdf62390-op[]ui'k;\\,/nm.`"[keycode];
        }

        // Output for debugging
        printf("Simulating keypress for keycode: %d (char: %c)\n", keycode, c);

        // Post the events
        CGEventPost(kCGHIDEventTap, keyDown);
        usleep(10000);  // Small delay
        CGEventPost(kCGHIDEventTap, keyUp);
    }

    // Release resources
    if (keyDown) CFRelease(keyDown);
    if (keyUp) CFRelease(keyUp);
    if (source) CFRelease(source);
}

void cleanup(void) {
    // Close the UART device
    if (uart_fd >= 0) {
        close(uart_fd);
        uart_fd = -1;
    }

    printf("Resources cleaned up\n");
}