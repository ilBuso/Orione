#!/bin/bash

# Find the right port based on FTDI device
find_port() {
    # Try to find by FTDI identifier (macOS specific paths)
    PORT=$(ls -l /dev/tty.usbserial-* 2>/dev/null | head -1 | awk '{print $NF}')
    
    # If not found, try other naming patterns
    if [ -z "$PORT" ]; then
        PORT=$(ls -l /dev/tty.FTDI* 2>/dev/null | head -1 | awk '{print $NF}')
    fi
    
    # If still not found, try all usbserial devices
    if [ -z "$PORT" ]; then
        PORT=$(ls -l /dev/tty.usbserial* 2>/dev/null | head -1 | awk '{print $NF}')
    fi
    
    # Last resort - check system_profiler for FTDI devices
    if [ -z "$PORT" ]; then
        if system_profiler SPUSBDataType | grep -q "FTDI"; then
            # FTDI device is connected but we couldn't find the port
            # Check all serial ports
            for p in $(ls /dev/tty.*); do
                if [[ $p == *"usbserial"* || $p == *"FTDI"* ]]; then
                    PORT=$p
                    break
                fi
            done
        fi
    fi
    
    if [ -z "$PORT" ]; then
        echo "ERROR: Could not find any FTDI device"
        exit 1
    fi
    
    # Verify the port exists
    if [ ! -e "$PORT" ]; then
        echo "ERROR: Port $PORT does not exist"
        exit 1
    fi
    
    echo $PORT
}

# Check if driver is already runningq
check_running() {
    PID=$(pgrep -f "orionev1-driver-macos")
    if [ ! -z "$PID" ]; then
        echo "Orione-v1 driver is already running (PID: $PID)"
        exit 0
    fi
}

# Main function
main() {
    check_running
    
    # Find the port
    PORT=$(find_port)
    echo "Found FTDI device on port: $PORT"
    
    # Check for permissions
    if [ ! -w "$PORT" ]; then
        echo "ERROR: No permission to access $PORT"
        echo "You may need to adjust permissions or run the install script again"
        exit 1
    fi
    
    # Launch the driver with the found port
    echo "Starting Orione-v1 driver..."
    /usr/local/bin/orionev1-driver-macos "$PORT"
    
    # If we got here, the driver exited
    echo "Driver exited with code $?"
}

# Run the main function
main