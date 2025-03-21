/**
* uart_to_keyboard_events_win.c - UART to Keyboard Event Simulator for Windows
*
* This program listens for UART input on a specified port and simulates
* keyboard events using the Windows API.
*
* Compile with: gcc -o uart_to_keyboard_events_win.c -lwinmm -lSetupAPI
*
* Run with: uart_to_keyboard_events_win.exe COM3 9600
* (Replace COM3 with your actual UART device)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Global variables
HANDLE hSerial = INVALID_HANDLE_VALUE;

// Function prototypes
HANDLE setup_uart(const char *port, int baud);
void simulate_key_press(char c);
void cleanup(void);

int main(int argc, char *argv[]) {
    // Extract command line arguments
    // Change to your port
    const char *uart_device = "";
    int baud_rate = 115200;

    // Prepend "\\.\\" to the COM port name for Windows API
    char full_port_name[20];
    sprintf(full_port_name, "\\\\.\\%s", uart_device);

    // Setup UART
    hSerial = setup_uart(full_port_name, baud_rate);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Failed to open UART device %s\n", uart_device);
        return 1;
    }
    printf("UART device %s opened successfully at %d baud\n", uart_device, baud_rate);

    // Register cleanup function
    atexit(cleanup);

    // Main loop to read UART and send keyboard events
    printf("Starting UART to keyboard simulator. Press Ctrl+C to exit.\n");
    char buffer[256];
    DWORD bytes_read;

    while (1) {
        // Read data from serial port
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytes_read, NULL)) {
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("Received: %s\n", buffer);

                // Process each character
                for (int i = 0; i < (int)bytes_read; i++) {
                    simulate_key_press(buffer[i]);
                    Sleep(10);  // 10ms delay between key presses
                }
            }
        } else {
            DWORD error = GetLastError();
            fprintf(stderr, "Error reading from serial port: %lu\n", error);
            Sleep(1000);  // Wait before retrying
        }

        Sleep(10);  // Small delay to prevent CPU hogging
    }

    return 0;
}

HANDLE setup_uart(const char *port, int baud) {
    HANDLE h_serial;
    DCB dcb = {0};
    COMMTIMEOUTS timeouts = {0};

    // Open the serial port
    h_serial = CreateFileA(
        port,                      // Port name
        GENERIC_READ | GENERIC_WRITE, // Read/Write access
        0,                         // No sharing
        NULL,                      // No security attributes
        OPEN_EXISTING,             // Must use OPEN_EXISTING
        0,                         // No overlapped I/O
        NULL                       // No template
    );

    if (h_serial == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        fprintf(stderr, "Error opening serial port: %lu\n", error);
        return INVALID_HANDLE_VALUE;
    }

    // Get current DCB settings
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(h_serial, &dcb)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Error getting comm state: %lu\n", error);
        CloseHandle(h_serial);
        return INVALID_HANDLE_VALUE;
    }

    // Configure DCB settings
    dcb.BaudRate = baud;           // Set baud rate
    dcb.ByteSize = 8;              // 8 data bits
    dcb.Parity = NOPARITY;         // No parity
    dcb.StopBits = ONESTOPBIT;     // 1 stop bit
    dcb.fBinary = TRUE;            // Binary mode
    dcb.fParity = FALSE;           // No parity checking
    dcb.fOutxCtsFlow = FALSE;      // No CTS flow control
    dcb.fOutxDsrFlow = FALSE;      // No DSR flow control
    dcb.fDtrControl = DTR_CONTROL_ENABLE; // Enable DTR
    dcb.fDsrSensitivity = FALSE;   // Don't monitor DSR
    dcb.fOutX = FALSE;             // No XON/XOFF flow control
    dcb.fInX = FALSE;              // No XON/XOFF flow control
    dcb.fErrorChar = FALSE;        // Don't replace errors
    dcb.fRtsControl = RTS_CONTROL_ENABLE; // Enable RTS
    dcb.fAbortOnError = FALSE;     // Don't abort on error

    // Apply DCB settings
    if (!SetCommState(h_serial, &dcb)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Error setting comm state: %lu\n", error);
        CloseHandle(h_serial);
        return INVALID_HANDLE_VALUE;
    }

    // Configure timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    // Apply timeout settings
    if (!SetCommTimeouts(h_serial, &timeouts)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Error setting comm timeouts: %lu\n", error);
        CloseHandle(h_serial);
        return INVALID_HANDLE_VALUE;
    }

    // Clear the comm buffers
    PurgeComm(h_serial, PURGE_TXCLEAR | PURGE_RXCLEAR);

    return h_serial;
}

void simulate_key_press(char c) {
    INPUT input[2];
    WORD vk;

    // Convert ASCII to virtual key code
    // For most standard keys, the virtual key code is the uppercase ASCII value
    if (c >= 'a' && c <= 'z') {
        vk = toupper(c);
    } else if (c >= 'A' && c <= 'Z' || c >= '0' && c <= '9') {
        vk = c;
    } else {
        // Handle special characters
        switch (c) {
            case '\r':
            case '\n': vk = VK_RETURN; break;
            case '\t': vk = VK_TAB; break;
            case '\b': vk = VK_BACK; break;
            case ' ': vk = VK_SPACE; break;
            case '!': vk = '1'; break;
            case '@': vk = '2'; break;
            case '#': vk = '3'; break;
            case '$': vk = '4'; break;
            case '%': vk = '5'; break;
            case '^': vk = '6'; break;
            case '&': vk = '7'; break;
            case '*': vk = '8'; break;
            case '(': vk = '9'; break;
            case ')': vk = '0'; break;
            case '-': vk = VK_OEM_MINUS; break;
            case '_': vk = VK_OEM_MINUS; break;
            case '=': vk = VK_OEM_PLUS; break;
            case '+': vk = VK_OEM_PLUS; break;
            case '[': vk = VK_OEM_4; break;
            case '{': vk = VK_OEM_4; break;
            case ']': vk = VK_OEM_6; break;
            case '}': vk = VK_OEM_6; break;
            case '\\': vk = VK_OEM_5; break;
            case '|': vk = VK_OEM_5; break;
            case ';': vk = VK_OEM_1; break;
            case ':': vk = VK_OEM_1; break;
            case '\'': vk = VK_OEM_7; break;
            case '"': vk = VK_OEM_7; break;
            case ',': vk = VK_OEM_COMMA; break;
            case '<': vk = VK_OEM_COMMA; break;
            case '.': vk = VK_OEM_PERIOD; break;
            case '>': vk = VK_OEM_PERIOD; break;
            case '/': vk = VK_OEM_2; break;
            case '?': vk = VK_OEM_2; break;
            case '`': vk = VK_OEM_3; break;
            case '~': vk = VK_OEM_3; break;
            default: return; // Unknown character, skip it
        }
    }

    // Check if we need to simulate Shift for uppercase or special characters
    BOOL need_shift = (c >= 'A' && c <= 'Z') ||
                    (c == '!' || c == '@' || c == '#' || c == '$' || c == '%' ||
                    c == '^' || c == '&' || c == '*' || c == '(' || c == ')' ||
                    c == '_' || c == '+' || c == '{' || c == '}' || c == '|' ||
                    c == ':' || c == '"' || c == '<' || c == '>' || c == '?' || c == '~');

    // Initialize the input structure
    ZeroMemory(input, sizeof(input));

    // First input for key down
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = vk;
    input[0].ki.dwFlags = 0; // 0 for key press

    // Second input for key up
    input[1].type = INPUT_KEYBOARD;
    input[1].ki.wVk = vk;
    input[1].ki.dwFlags = KEYEVENTF_KEYUP;

    // If shift is needed, simulate shift key press first
    if (need_shift) {
        INPUT shift_down = {0};
        shift_down.type = INPUT_KEYBOARD;
        shift_down.ki.wVk = VK_SHIFT;
        shift_down.ki.dwFlags = 0;
        SendInput(1, &shift_down, sizeof(INPUT));
        Sleep(5);
    }

    // Send key down and key up events
    SendInput(2, input, sizeof(INPUT));

    // If shift was pressed, release it
    if (need_shift) {
        INPUT shift_up = {0};
        shift_up.type = INPUT_KEYBOARD;
        shift_up.ki.wVk = VK_SHIFT;
        shift_up.ki.dwFlags = KEYEVENTF_KEYUP;
        Sleep(5);
        SendInput(1, &shift_up, sizeof(INPUT));
    }

    // Output for debugging
    printf("Simulating keypress for key: '%c' (VK: 0x%02X)\n", c, vk);
}

void cleanup(void) {
    // Close the serial port
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }

    printf("Resources cleaned up\n");
}