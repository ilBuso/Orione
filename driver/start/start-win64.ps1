# Function to find the COM port for the FTDI device
function Find-FTDIPort {
    Write-Host "Finding FTDI device COM port..." -ForegroundColor Yellow
    
    # Query Windows Management Instrumentation for COM ports
    $comPorts = Get-WmiObject -Query "SELECT * FROM Win32_PnPEntity WHERE (Caption LIKE '%COM%' AND Caption LIKE '%FTDI%')"
    
    if (-not $comPorts) {
        # Try alternative approach if WMI query fails
        $comPorts = Get-ItemProperty -Path "HKLM:\HARDWARE\DEVICEMAP\SERIALCOMM" -ErrorAction SilentlyContinue | 
                    Select-Object -Property * -ExcludeProperty PSPath, PSParentPath, PSChildName, PSDrive, PSProvider |
                    ForEach-Object { $_.PSObject.Properties } | 
                    Where-Object { $_.Value -like "*COM*" }
        
        # If still not found, search for any COM port as a last resort
        if (-not $comPorts) {
            $comPorts = Get-WmiObject -Query "SELECT * FROM Win32_PnPEntity WHERE Caption LIKE '%COM%'"
        }
    }
    
    if (-not $comPorts) {
        Write-Host "ERROR: Could not find any COM port devices" -ForegroundColor Red
        return $null
    }
    
    # If multiple COM ports, try to find the FTDI specifically
    $ftdiPort = $null
    foreach ($port in $comPorts) {
        if ($port.Caption -like "*FTDI*" -or $port.Description -like "*FTDI*" -or $port.Name -like "*FTDI*") {
            # Extract COM port number
            if ($port.Caption -match "COM(\d+)") {
                $ftdiPort = "COM" + $matches[1]
                break
            } elseif ($port.Name -match "COM(\d+)") {
                $ftdiPort = "COM" + $matches[1]
                break
            } else {
                # Try to find COM port in the value directly (for registry method)
                $ftdiPort = $port.Value
                break
            }
        }
    }
    
    # If still no match, just take the first COM port
    if (-not $ftdiPort -and $comPorts.Count -gt 0) {
        if ($comPorts[0].Caption -match "COM(\d+)") {
            $ftdiPort = "COM" + $matches[1]
        } elseif ($comPorts[0].Name -match "COM(\d+)") {
            $ftdiPort = "COM" + $matches[1]
        } else {
            # Try to find COM port in the value directly (for registry method)
            $ftdiPort = $comPorts[0].Value
        }
    }
    
    if (-not $ftdiPort) {
        Write-Host "ERROR: Could not determine COM port for FTDI device" -ForegroundColor Red
        return $null
    }
    
    Write-Host "Found FTDI device on port: $ftdiPort" -ForegroundColor Green
    return $ftdiPort
}

# Function to check if driver is already running
function Check-DriverRunning {
    $process = Get-Process -Name "orionev1-driver-windows" -ErrorAction SilentlyContinue
    
    if ($process) {
        Write-Host "Orione-v1 driver is already running (PID: $($process.Id))" -ForegroundColor Yellow
        return $true
    }
    
    return $false
}

# Main function
function Start-Driver {
    # Check if already running
    if (Check-DriverRunning) {
        return
    }
    
    # Find the COM port
    $port = Find-FTDIPort
    
    if (-not $port) {
        Write-Host "Could not find a suitable COM port. Is the keyboard connected?" -ForegroundColor Red
        exit 1
    }
    
    # Launch the driver with the found port
    $driverPath = "C:\Program Files\Orione-v1\orionev1-driver-windows.exe"
    
    if (-not (Test-Path $driverPath)) {
        Write-Host "ERROR: Driver executable not found at $driverPath" -ForegroundColor Red
        exit 1
    }
    
    Write-Host "Starting Orione-v1 driver on port $port..." -ForegroundColor Green
    
    try {
        # Start the process and wait for it to exit
        $process = Start-Process -FilePath $driverPath -ArgumentList $port -NoNewWindow -PassThru
        
        # Create a log file for diagnostics
        $logFile = "C:\Program Files\Orione-v1\driver.log"
        "$(Get-Date) - Started driver with port $port (PID: $($process.Id))" | Out-File -Append $logFile
        
        # Wait for process to exit
        $process.WaitForExit()
        
        # Log the exit
        "$(Get-Date) - Driver exited with code $($process.ExitCode)" | Out-File -Append $logFile
        
        Write-Host "Driver exited with code $($process.ExitCode)" -ForegroundColor Yellow
    }
    catch {
        Write-Host "ERROR: Failed to start driver: $_" -ForegroundColor Red
        "$(Get-Date) - ERROR: Failed to start driver: $_" | Out-File -Append $logFile
        exit 1
    }
}

# Run the main function
Start-Driver