//
// Created by Giulio Pimenoff Verdolin on 08/05/25.
//

#pragma once
#include "message.h"

// Initializes uart, needs to be implemented differently for each target OS
// This function should set up the necessary hardware or software configurations
// for UART communication. The implementation is platform-dependent and will
// differ based on the target operating system.
// Returns:
//    - 0 on success,
//    - Non-zero error code on failure.
int uart_init();

/**
 * Reads a single byte from the UART interface.
 *
 * Attempts to read one byte from the UART device specified in uart_init().
 * If a byte is successfully read, it is returned. If no data is read or an
 * error occurs, the function returns 255 as an error indicator.
 *
 * Returns:
 *   - The received byte (0–254) if successful.
 *   - 255 if no data was read or an error occurred.
 */
uint8_t uart_read_byte();

// Cleans up and closes the UART interface
// This function is used to release any resources that were acquired during
// UART initialization and usage. It may also handle the closure of UART
// connections or stopping of any relevant services.
void uart_cleanup();