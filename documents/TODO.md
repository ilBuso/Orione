# ToDo List
## Yet to implement
- **Debouncing**: Debouncing is a process that involves a **timer** of _n_ seconds, which activates when a key is pressed. This mechanism prevents the unintended repeated sending of the same character due to the natural mechanical vibrations, electrical noise or human reflexes that occur when a key is pressed or released.
- **Special Keys**: Understand and implement the `Function Keys`, `Esc`, `Canc` ...
- **Caps-Lock**: Understand and implement the `Caps-Lock` key
- **Multiple Layers**: Understand how to handle and implement all the layer keys (ex. `Shift`, `Ctrl`, `Alt`, `AltGr` ...)
- **Multimedia Keys**: Understand and implement the Multimedia keys (Stop/Play Audio, Increase/Decrease Brightness ...)
- **USB HID Emulator**: Due to the nature of the USB-to-UART cable, we need an **emulator** that converts the data coming from the computer's **serial port** into **key-binds**, making it usable as keyboard input. To achieve this, we need to write a USB-to-HID emulator (HID = Human Interface Device). I have already drafted an initial version in `vk.c`. While it currently functions, it requires further improvements to enhance efficiency. For example, replacing obsolete functions such as `open()` with `fopen()`.
- **OLED-Display**:
   - Uses I2C so we will need to setup the communication
   - Decide what to show on it (ex. ButtonPressedPerMinute, Volume ...)
- **Rotary Encoder**: Understand and implement the volume knob

## Bugs
- **Interrupt**: continuously sending interrupt to to `scan_row()` not disabling the interrupts so every it loops to find the row that originated the interrupt it sends another one. WE MUST CHECK THAT DISABILITATE THE INTERRUPT DOES NOT INTERFERE WIT THE CLICK OF OTHER KEYS.

## Nice to Add
- **Macros**: Useful macros (keep in mind that it is not a commercial keyboard just for a selected user)
- **Dynamic Key-Mapping**: Feature that let user import it's own Key-Map (keep in mind that it is not a commercial keyboard just for a selected user)
- **Daemon**: make the USB-HID a daemon
- **GUI**: to update the key-map use a gui to upload (see if it is even possible)
- [**3D-Printer**](https://fablab.unitn.it/): 
   - Key-Caps
   - Case

# Delivery
- README.md 
- Pitch video
- Presentation
