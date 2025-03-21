#!/bin/bash

# Find the right port based on FTDI device
find_port() {
    # Try to find by FTDI identifier
    PORT=$(ls -l /dev/serial/by-id/*FTDI* 2>/dev/null | grep -o "tty[^ ]*$" | head -1)
    
    # If not found, try by manufacturer string
    if [ -z "$PORT" ]; then
        PORT=$(ls -l /dev/serial/by-manufacturer/*FTDI* 2>/dev/null | grep -o "tty[^ ]*$" | head -1)
    fi
    
    # If still not found, try checking all USB serial devices and grep for FTDI
    if [ -z "$PORT" ]; then
        for DEVICE in /sys/bus/usb-serial/devices/*; do
            if [ -e "$DEVICE/../../manufacturer" ]; then
                if grep -q "FTDI" "$DEVICE/../../manufacturer"; then
                    PORT=$(basename $(readlink -f "$DEVICE/tty"))
                    break
                fi
            fi
        done
    fi
    
    # Last resort - check dmesg for recent FTDI connections
    if [ -z "$PORT" ]; then
        PORT=$(dmesg | grep -i "FTDI.*tty" | tail -1 | grep -o "tty[^ :,]*" | head -1)
    fi
    
    if [ -z "$PORT" ]; then
        echo "ERROR: Could not find any FTDI device"
        exit 1
    fi
    
    # Make sure we have the full path
    if [[ "$PORT" != "/dev/"* ]]; then
        PORT="/dev/$PORT"
    fi
    
    # Verify the port exists
    if [ ! -e "$PORT" ]; then
        echo "ERROR: Port $PORT does not exist"
        exit 1
    fi
    
    echo $PORT
}

# Check if driver is already running
check_running() {
    PID=$(pgrep -f "orionev1-driver-linux")
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
        echo "Run 'sudo usermod -a -G dialout $USER' and reboot to fix permissions"
        exit 1
    fi
    
    # Launch the driver with the found port
    echo "Starting Orione-v1 driver..."
    /usr/local/bin/orionev1-driver-linux "$PORT"
    
    # If we got here, the driver exited
    echo "Driver exited with code $?"
}

# Run the main function
main