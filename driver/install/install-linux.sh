#!/bin/bash

# Ensure script is run as root
if [ "$EUID" -ne 0 ]; then
  echo "This script must be run as root (sudo)"
  exit 1
fi

# Define installation paths
INSTALL_DIR="/usr/local/bin"
SERVICE_DIR="/etc/systemd/system"
USERNAME=$(logname || echo "$SUDO_USER")
UDEV_RULES_DIR="/etc/udev/rules.d"

echo "=== Installing Orione-v1 Driver ==="

# Check dependencies
check_dependencies() {
    echo "Checking dependencies..."
    
    # Check for required packages
    REQUIRED_PACKAGES="gcc make pkg-config libudev-dev libevdev-dev uinput"
    MISSING_PACKAGES=""
    
    for pkg in $REQUIRED_PACKAGES; do
        if ! dpkg -l | grep -q "$pkg"; then
            MISSING_PACKAGES="$MISSING_PACKAGES $pkg"
        fi
    done
    
    # Install missing Packages
    if [ ! -z "$MISSING_PACKAGES" ]; then
        # Initialize package manager list
        PACKAGE_MANAGERS=("apt" "yum" "dnf" "pacman" "zypper")

        # Find the first available package manager
        for pm in "${PACKAGE_MANAGERS[@]}"; do
            if command -v "$pm" >/dev/null 2>&1; then
                # Install Missing dependencies
                echo "Installing required packages:$MISSING_PACKAGES"
                $pm update
                $pm install -y $MISSING_PACKAGES
            fi
        done
    fi
}

# Install driver binary and script
install_files() {
    echo "Installing driver files..."
    
    # Create directories if they don't exist
    mkdir -p "$INSTALL_DIR"
    
    # Copy executable
    cp ../build/bin/orionev1-driver-linux "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/orionev1-driver-linux"
    
    # Copy start script
    cp ../start/start-linux.sh "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/start-linux.sh"
    
    # Set ownership
    chown root:root "$INSTALL_DIR/orionev1-driver-linux"
    chown root:root "$INSTALL_DIR/start-linux.sh"
    
    echo "Files installed successfully"
}

# Create a systemd service for automatic startup
create_service() {
    echo "Creating systemd service..."
    
    # Create service file
    cat > "$SERVICE_DIR/orione-keyboard.service" << EOF
[Unit]
Description=Orione Keyboard Driver
After=multi-user.target

[Service]
Type=simple
User=$USERNAME
ExecStart=$INSTALL_DIR/start-linux.sh
Restart=on-failure
RestartSec=5

[Install]
WantedBy=multi-user.target
EOF
    
    # Reload systemd, enable and start service
    systemctl daemon-reload
    systemctl enable orione-keyboard.service
    systemctl start orione-keyboard.service
    
    echo "Service created and started."
}

# Create udev rules for device permissions
create_udev_rules() {
    echo "Creating udev rules for device permissions..."
    
    # Create rule for MSP432 USB device (adjust VID/PID to match your device)
    cat > "$UDEV_RULES_DIR/99-orione-keyboard.rules" << EOF
# Orione Keyboard device rule
SUBSYSTEM=="tty", ATTRS{idVendor}=="0x0403", ATTRS{idProduct}=="0x6001", MODE="0666", GROUP="dialout", SYMLINK+="orione_keyboard"
EOF
    
    # Reload udev rules
    udevadm control --reload-rules
    udevadm trigger
    
    # Add user to dialout group (for serial port access)
    usermod -a -G dialout "$USERNAME"
    
    echo "udev rules created. User added to dialout group."
    echo "NOTE: You may need to log out and back in for group changes to take effect."
}

# Configure uinput permissions
configure_uinput() {
    echo "Configuring uinput permissions..."
    
    # Create udev rule for uinput access
    cat > "$UDEV_RULES_DIR/99-uinput.rules" << EOF
# uinput device permissions
KERNEL=="uinput", MODE="0660", GROUP="input", OPTIONS+="static_node=uinput"
EOF
    
    # Reload udev rules
    udevadm control --reload-rules
    udevadm trigger
    
    # Add user to input group
    usermod -a -G input "$USERNAME"
    
    echo "uinput permissions configured."
}

# Verify installation
verify_installation() {
    echo "Verifying installation..."
    
    if [ ! -x "$INSTALL_DIR/orionev1-driver-linux" ]; then
        echo "ERROR: Driver executable not installed correctly"
        return 1
    fi
    
    if [ ! -x "$INSTALL_DIR/start-linux.sh" ]; then
        echo "ERROR: Start script not installed correctly"
        return 1
    fi
    
    if ! systemctl list-unit-files | grep -q "orione-keyboard.service"; then
        echo "WARNING: Systemd service not installed correctly"
    fi
    
    echo "Installation verified successfully."
    return 0
}

# Main installation process
main() {
    check_dependencies
    install_files
    create_udev_rules
    configure_uinput
    create_service
    verify_installation
    
    echo ""
    echo "=== Installation Complete ==="
    echo "The Orione-v1 driver has been installed and configured to start automatically."
    echo "To manually start/stop the driver, use:"
    echo "  systemctl start orione-keyboard.service"
    echo "  systemctl stop orione-keyboard.service"
    echo ""
    echo "To check status, use:"
    echo "  systemctl status orione-keyboard.service"
    echo ""
    echo "You may need to reboot for all permission changes to take effect."
}

# Run the main function
main