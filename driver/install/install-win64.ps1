# Orione-v1 Keyboard Driver Installation Script for Windows
# Run as Administrator

# Check if running as Administrator
$currentPrincipal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
$isAdmin = $currentPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)

if (-not $isAdmin) {
    Write-Host "This script must be run as Administrator" -ForegroundColor Red
    exit 1
}

# Define installation paths
$installDir = "C:\Program Files\Orione-v1"
$driverExe = "orionev1-driver-windows.exe"
$startScript = "start-windows.ps1"
$taskName = "Orione-Keyboard-Driver"

Write-Host "=== Installing Orione-v1 Keyboard Driver for Windows ===" -ForegroundColor Cyan

# Function to check and install dependencies
function Check-Dependencies {
    Write-Host "Checking dependencies..." -ForegroundColor Yellow
    
    # Check for Visual C++ Redistributable (required by many drivers)
    $vcRedistInstalled = Get-WmiObject -Class Win32_Product | Where-Object { $_.Name -like "*Microsoft Visual C++*" }
    
    if (-not $vcRedistInstalled) {
        Write-Host "Microsoft Visual C++ Redistributable not found. Installing..." -ForegroundColor Yellow
        
        # Download and install VC++ Redistributable
        $vcRedistUrl = "https://aka.ms/vs/17/release/vc_redist.x64.exe"
        $vcRedistInstaller = "$env:TEMP\vc_redist.x64.exe"
        
        Invoke-WebRequest -Uri $vcRedistUrl -OutFile $vcRedistInstaller
        Start-Process -FilePath $vcRedistInstaller -ArgumentList "/quiet", "/norestart" -Wait
        
        Remove-Item $vcRedistInstaller -Force
    }
}

# Function to install driver files
function Install-DriverFiles {
    Write-Host "Installing driver files..." -ForegroundColor Yellow
    
    # Create installation directory if it doesn't exist
    if (-not (Test-Path -Path $installDir)) {
        New-Item -Path $installDir -ItemType Directory -Force | Out-Null
    }
    
    # Copy driver executable
    Copy-Item -Path "..\build\bin\$driverExe" -Destination "$installDir\$driverExe" -Force
    
    # Copy start script
    Copy-Item -Path "..\start\$startScript" -Destination "$installDir\$startScript" -Force
    
    # Create a batch file wrapper for the PowerShell script (easier for Task Scheduler)
    $batchContent = @"
@echo off
PowerShell -NoProfile -ExecutionPolicy Bypass -File "$installDir\$startScript"
"@
    
    Set-Content -Path "$installDir\start-orione.bat" -Value $batchContent -Force
    
    Write-Host "Files installed successfully" -ForegroundColor Green
}

# Function to install required drivers for FTDI chip
function Install-FTDIDrivers {
    Write-Host "Setting up FTDI device drivers..." -ForegroundColor Yellow
    
    # Check if FTDI drivers are already installed
    $ftdiDrivers = Get-WmiObject -Class Win32_PnPSignedDriver | Where-Object { $_.Description -like "*FTDI*" }
    
    if (-not $ftdiDrivers) {
        Write-Host "FTDI drivers not detected. Please download and install FTDI drivers from:" -ForegroundColor Yellow
        Write-Host "https://ftdichip.com/drivers/vcp-drivers/" -ForegroundColor Cyan
        
        # Ask if user wants to download the driver
        $download = Read-Host "Do you want to open the FTDI driver download page? (Y/N)"
        
        if ($download -eq "Y" -or $download -eq "y") {
            Start-Process "https://ftdichip.com/drivers/vcp-drivers/"
            Write-Host "Please install the drivers and then press Enter to continue..." -ForegroundColor Yellow
            Read-Host
        }
    } else {
        Write-Host "FTDI drivers already installed." -ForegroundColor Green
    }
}

# Function to create a scheduled task for auto-start
function Create-ScheduledTask {
    Write-Host "Creating scheduled task for automatic startup..." -ForegroundColor Yellow
    
    # Remove existing task if present
    $existingTask = Get-ScheduledTask -TaskName $taskName -ErrorAction SilentlyContinue
    if ($existingTask) {
        Unregister-ScheduledTask -TaskName $taskName -Confirm:$false
    }
    
    # Create a new scheduled task
    $action = New-ScheduledTaskAction -Execute "C:\Windows\System32\cmd.exe" -Argument "/c `"$installDir\start-orione.bat`""
    $trigger = New-ScheduledTaskTrigger -AtLogOn
    $principal = New-ScheduledTaskPrincipal -UserId "SYSTEM" -LogonType ServiceAccount -RunLevel Highest
    $settings = New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries -RestartCount 3 -RestartInterval (New-TimeSpan -Minutes 1)
    
    Register-ScheduledTask -TaskName $taskName -Action $action -Trigger $trigger -Principal $principal -Settings $settings -Force
    
    # Start the task now
    Start-ScheduledTask -TaskName $taskName
    
    Write-Host "Scheduled task created and started." -ForegroundColor Green
}

# Function to configure COM port permissions
function Configure-COMPortPermissions {
    Write-Host "Configuring COM port permissions..." -ForegroundColor Yellow
    
    # Get the current user's SID
    $user = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name
    
    # Create a registry key for COM port permissions
    $regPath = "HKLM:\SYSTEM\CurrentControlSet\Enum\FTDIBUS"
    
    if (Test-Path $regPath) {
        Get-ChildItem $regPath -Recurse | ForEach-Object {
            $deviceParams = "$($_.PSPath)\Device Parameters"
            if (Test-Path $deviceParams) {
                # Set permissions for the COM port
                Write-Host "Setting permissions for $deviceParams" -ForegroundColor Yellow
                $acl = Get-Acl $deviceParams
                $rule = New-Object System.Security.AccessControl.RegistryAccessRule($user, "FullControl", "Allow")
                $acl.SetAccessRule($rule)
                Set-Acl $deviceParams $acl
            }
        }
    } else {
        Write-Host "FTDI devices not found in registry. Connect the device and run this script again." -ForegroundColor Yellow
    }
}

# Function to verify installation
function Verify-Installation {
    Write-Host "Verifying installation..." -ForegroundColor Yellow
    
    $success = $true
    
    if (-not (Test-Path -Path "$installDir\$driverExe")) {
        Write-Host "ERROR: Driver executable not installed correctly" -ForegroundColor Red
        $success = $false
    }
    
    if (-not (Test-Path -Path "$installDir\$startScript")) {
        Write-Host "ERROR: Start script not installed correctly" -ForegroundColor Red
        $success = $false
    }
    
    $task = Get-ScheduledTask -TaskName $taskName -ErrorAction SilentlyContinue
    if (-not $task) {
        Write-Host "WARNING: Scheduled task not installed correctly" -ForegroundColor Yellow
    }
    
    if ($success) {
        Write-Host "Installation verified successfully." -ForegroundColor Green
    }
    
    return $success
}

# Main installation process
function Main {
    Check-Dependencies
    Install-DriverFiles
    Install-FTDIDrivers
    Configure-COMPortPermissions
    Create-ScheduledTask
    $success = Verify-Installation
    
    Write-Host ""
    Write-Host "=== Installation Complete ===" -ForegroundColor Cyan
    Write-Host "The Orione-v1 driver has been installed and configured to start automatically." -ForegroundColor White
    Write-Host "To manually start/stop the driver, use Task Scheduler:" -ForegroundColor White
    Write-Host "  - Open Task Scheduler" -ForegroundColor White
    Write-Host "  - Find and right-click on '$taskName'" -ForegroundColor White
    Write-Host "  - Select 'Run' or 'End'" -ForegroundColor White
    Write-Host ""
    Write-Host "You may need to reboot for all changes to take effect." -ForegroundColor Yellow
    
    if (-not $success) {
        Write-Host "Some problems were detected during installation. Please check the error messages above." -ForegroundColor Red
    }
}

# Run the main function
Main