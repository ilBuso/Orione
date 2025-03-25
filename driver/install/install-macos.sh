#!/bin/bash

# Ensure script is run with administrative privileges
if [ "$(id -u)" -ne 0 ]; then
  echo "This script must be run with sudo"
  exit 1
fi

# Define installation paths
INSTALL_DIR="/usr/local/bin"
LAUNCH_AGENTS_DIR="/Library/LaunchAgents"
USERNAME=$(logname || echo "$SUDO_USER")
USER_HOME=$(eval echo ~$USERNAME)

echo "=== Installing Orione-v1 Driver for macOS ==="

# Check dependencies
check_dependencies() {
    echo "Checking dependencies..."
    
    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo "Homebrew is not installed. Installing Homebrew..."
        # We need to run this as the user, not as root
        su - $USERNAME -c '/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"'
    fi
    
    # Check for required packages (using Homebrew)
    REQUIRED_PACKAGES="libusb pkg-config"
    MISSING_PACKAGES=""
    
    for pkg in $REQUIRED_PACKAGES; do
        if ! su - $USERNAME -c "brew list $pkg &> /dev/null"; then
            MISSING_PACKAGES="$MISSING_PACKAGES $pkg"
        fi
    done
    
    # Install missing packages
    if [ ! -z "$MISSING_PACKAGES" ]; then
        echo "Installing required packages:$MISSING_PACKAGES"
        su - $USERNAME -c "brew update && brew install $MISSING_PACKAGES"
    fi
}

# Install driver binary and script
install_files() {
    echo "Installing driver files..."
    
    # Create directories if they don't exist
    mkdir -p "$INSTALL_DIR"
    
    # Copy executable
    cp ../build/bin/orionev1-driver-macos "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/orionev1-driver-macos"
    
    # Copy start script
    cp ../start/start-macos.sh "$INSTALL_DIR/"
    chmod +x "$INSTALL_DIR/start-macos.sh"
    
    # Set ownership
    chown root:wheel "$INSTALL_DIR/orionev1-driver-macos"
    chown root:wheel "$INSTALL_DIR/start-macos.sh"
    
    echo "Files installed successfully"
}

# Create a launchd service for automatic startup
create_launchd_service() {
    echo "Creating launchd service..."
    
    # Create service file (plist)
    cat > "$LAUNCH_AGENTS_DIR/com.orione.keyboard.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.orione.keyboard</string>
    <key>ProgramArguments</key>
    <array>
        <string>$INSTALL_DIR/start-macos.sh</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
    <key>KeepAlive</key>
    <true/>
    <key>StandardErrorPath</key>
    <string>/tmp/orione-keyboard.err</string>
    <key>StandardOutPath</key>
    <string>/tmp/orione-keyboard.log</string>
</dict>
</plist>
EOF
    
    # Set ownership and permissions
    chown root:wheel "$LAUNCH_AGENTS_DIR/com.orione.keyboard.plist"
    chmod 644 "$LAUNCH_AGENTS_DIR/com.orione.keyboard.plist"
    
    # Load the service
    launchctl load -w "$LAUNCH_AGENTS_DIR/com.orione.keyboard.plist"
    
    echo "LaunchAgent created and loaded."
}

# Configure device permissions for macOS
configure_device_permissions() {
    echo "Configuring device permissions..."
    
    # In macOS, we don't need udev rules. Instead, we'll ensure the user
    # has the necessary permissions to access serial devices
    
    # Create/update a specific group for device access
    if ! dscl . -read /Groups/dialout &>/dev/null; then
        # Create dialout group if it doesn't exist (mimicking Linux convention)
        dscl . -create /Groups/dialout
        dscl . -create /Groups/dialout PrimaryGroupID 406  # Choose an unused GID
    fi
    
    # Add user to the dialout group
    dscl . -append /Groups/dialout GroupMembership $USERNAME
    
    echo "Device permissions configured."
    echo "NOTE: You may need to log out and back in for group changes to take effect."
}

# Verify installation
verify_installation() {
    echo "Verifying installation..."
    
    if [ ! -x "$INSTALL_DIR/orionev1-driver-macos" ]; then
        echo "ERROR: Driver executable not installed correctly"
        return 1
    fi
    
    if [ ! -x "$INSTALL_DIR/start-macos.sh" ]; then
        echo "ERROR: Start script not installed correctly"
        return 1
    fi
    
    if [ ! -f "$LAUNCH_AGENTS_DIR/com.orione.keyboard.plist" ]; then
        echo "WARNING: LaunchAgent not installed correctly"
    fi
    
    echo "Installation verified successfully."
    return 0
}

# Main installation process
main() {
    check_dependencies
    install_files
    configure_device_permissions
    create_launchd_service
    verify_installation
    
    echo ""
    echo "=== Installation Complete ==="
    echo "The Orione-v1 driver has been installed and configured to start automatically."
    echo "To manually start/stop the driver, use:"
    echo "  launchctl load -w $LAUNCH_AGENTS_DIR/com.orione.keyboard.plist"
    echo "  launchctl unload -w $LAUNCH_AGENTS_DIR/com.orione.keyboard.plist"
    echo ""
    echo "To check if the service is running, use:"
    echo "  launchctl list | grep orione"
    echo ""
    echo "You may need to reboot for all permission changes to take effect."
}

# Run the main function
main