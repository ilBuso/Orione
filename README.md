# Orione

A custom 60% mechanical keyboard powered by Raspberry Pi Pico with fully customizable firmware and dual-layer functionality.

## TOC

- [TOC](#toc)
- [About](#about)
- [Components](#components)
  - [Hardware](#hardware)
  - [Software](#software)
- [PCB](#pcb)
- [Chassis](#chassis)
- [Project layout](#project-layout)
- [Video and Presentation](#video-and-presentation)
- [Team Members](#team-members)
- [Resources](#resources)

## About

Orione is a compact 60% keyboard layout built around the Raspberry Pi Pico microcontroller. It features a programmable dual-layer system accessible via a dedicated function key, allowing you to maximize functionality while maintaining a minimal footprint.

## Features

- **60% Layout**: Compact design without F-row function keys
- **Dual Layer System**: Access additional functions through a dedicated function key
- **Fully Customizable Firmware**: Modify keymaps to suit your workflow
- **Rotary Encoder**: Volume control with raise, lower, and mute functionality
- **Raspberry Pi Pico**: Reliable and affordable microcontroller
- **Gateron Yellow Switches**: Smooth linear switches for most keys
- **Gateron Blue Switches**: Tactile feedback on select keys
- **Caps Lock LED Indicator**: Visual feedback for Caps Lock status

## Customization

One of Orione's key features is its fully customizable firmware. You can modify the keymap to match your preferences:

1. Edit the keymap configuration files
2. Recompile the firmware
3. Flash the new firmware to your Raspberry Pi Pico
4. Enjoy your personalized keyboard layout

## Components

### Hardware

- **Raspberry Pi Pico** (1x) - Microcontroller
- **Rotary Encoder** (1x) - Volume control
- **220Ω Resistor** (1x) - For LED current limiting
- **LED** (1x) - Caps Lock indicator
- **Mechanical Switches** (68x) - Gateron Yellow and Blue
- **Keycaps Set** (68x) - Compatible with your switch choice
- **Diodes** (68x) - 1N4148 or similar for key matrix
- **Custom PCB** (1x) - Designed by us

### Software

- **Development Environment**: VS Code
- **KiCad** - PCB schematics and layout design
- **Blender** - 3D modeling for case and components  
  
- **SDK**: Raspberry Pi Pico C/C++ SDK (follow the official Getting Started guide)
- **TinyUSB Library**: Used to enable USB HID communication between the Pico and computer via the micro USB port

## PCB

## Chassis

## Project layout

```
/
├───datasheets
|   └───...
├───firmware
│   ├───config
│   └───src
│       ├───init
│       │   ├───init.h
│       │   └───init.c
│       ├───interrupts
│       │   ├───interrupts.h
│       │   └───interrupts.c
│       ├───matrix
│       │   ├───keymap
│       │   │   ├───keymap.h
│       │   │   └───keymap.c
│       │   └───scan_rows
│       │       ├───scan_rows.h
│       │       └───scan_rows.c
│       ├───rotary_encoder
│       │   ├───rotary_encoder.h
│       │   └───rotary_encoder.c
│       └───usb
│           ├───usb_callbacks
│           │  ├───usb_callbacks.h
│           │  └───usb_callbacks.c
│           └───usb_descriptors
│              ├───usb_descriptors.h
│              └───usb_descriptors.c
├───pcb
│    └───orione-v2
│        └───...
└───chassis
    └───...
```

## Video and Presentation

## Team Members

## Resources
