# To Do
Things to do in the various stages of the development

# R&D
The plan is to first make sure that thr interrupts work by lighting up a led in the MCU the immediately understand how to use the USB so that all the rest of the Development is easier. So in the end after asserting that the idea is right whe immediately work on the USB and then we actually work on the keyboard.

## Software
- [ ] Basic Functions --> Matrix, Pressing button
- [ ] Layers
- [ ] Caps Lock lights up Led
- [ ] USB Data
- [ ] Light up Led when powered --> see if more useful to do it HW

### How a Switch Matrix Works
1. **Rows and Columns**:
   - The switch matrix is arranged in a grid where switches connect rows to columns.
   - Each switch is located at the intersection of a row and a column.

2. **Scanning the Matrix**:
   - To detect which switch is pressed, you:
     - Drive one set of pins (e.g., rows) as **outputs**.
     - Configure the other set of pins (e.g., columns) as **inputs**.
     - Activate each row sequentially and read the state of the columns to determine which switch is closed.

3. **No Pull-Up/Pull-Down on All Pins**:
   - Using pull-up or pull-down resistors directly on the matrix pins can cause false readings because current may "leak" through multiple paths when multiple switches are pressed.
   - Instead, the microcontroller must actively control the row and column pin states during scanning.

### Configuring Pins in a Switch Matrix
#### Pin States:
- **Rows (Outputs)**:
  - These pins are set as outputs in the `PxDIR` register.
  - During scanning:
    - One row pin is driven low (`0`) at a time.
    - Other row pins are set to high-impedance or high (`1`).

- **Columns (Inputs)**:
  - These pins are set as inputs in the `PxDIR` register.
  - Use the `PxIN` register to read the state of the columns.
  - **Enable internal pull-up resistors** on the column pins to ensure they default to `1` when no switch is pressed.


### Steps to Configure a Switch Matrix
1. **Configure Rows as Outputs**:
   - For example, if `P1.0`, `P1.1`, and `P1.2` are row pins:
     ```c
     P1->DIR |= (BIT0 | BIT1 | BIT2);  // Set rows (P1.0, P1.1, P1.2) as outputs
     ```

2. **Configure Columns as Inputs**:
   - For example, if `P2.0`, `P2.1`, and `P2.2` are column pins:
     ```c
     P2->DIR &= ~(BIT0 | BIT1 | BIT2);  // Set columns (P2.0, P2.1, P2.2) as inputs
     ```

3. **Enable Pull-Up Resistors on Columns**:
   - Pull-ups ensure the column pins read high (`1`) when no switch is pressed:
     ```c
     P2->OUT |= (BIT0 | BIT1 | BIT2);  // Enable pull-up resistors
     P2->REN |= (BIT0 | BIT1 | BIT2);  // Activate resistors
     ```

4. **Scan the Matrix**:
   - Sequentially drive each row low (`0`), while others are high (`1` or high-Z).
   - Read the column pins to detect which switch (if any) is pressed:
     ```c
     // Example: Scanning a 3x3 matrix
     for (int row = 0; row < 3; row++) {
         // Drive the current row low
         P1->OUT &= ~(1 << row);  // Clear the bit for the active row
         P1->OUT |= (~(1 << row) & (BIT0 | BIT1 | BIT2));  // Set other rows high

         // Read the columns
         uint8_t col_state = P2->IN & (BIT0 | BIT1 | BIT2);

         // Process the column states for the current row
         // ...
     }
     ```

### **Debouncing**
Since mechanical switches (like Cherry MX) can bounce (i.e., rapidly open and close during a press), you must add **debouncing** logic:
- Either use a small delay (software debounce).
- Or implement more robust algorithms to confirm stable state changes.

## UART-USB
### Pin Configuration
1. **TXD (Transmit Data)**:
    - This is the output from the MSP432 to transmit data to the UART-to-USB cable.
    - You need to configure the pin connected to TXD as an output.
    - You will use the alternate function mode for this pin to enable UART functionality.

2. **RXD (Receive Data)**:
    - This is the input to the MSP432 to receive data from the UART-to-USB cable.
    - You need to configure the pin connected to RXD as an input.
    - You will also use the alternate function mode for this pin to enable UART functionality.

### Notes
1. **Active Low Signals**:
   - RTS# and CTS# are often active low. This means a low voltage level indicates readiness. Confirm the polarity of the signals from the UART-to-USB cable documentation.

2. **Flow Control Requirements**:
   - If hardware flow control is not used, you can leave RTS# and CTS# unconnected or configure them as GPIOs for other purposes.

3. **Check the Documentation**:
   - Refer to the MSP432P401R and UART-to-USB cable documentation for electrical specifications, such as voltage levels, to ensure compatibility.

By configuring the pins appropriately, you can use hardware flow control effectively or disable it if unnecessary.

## Hardware
- [ ] Buy Parts
    - [ ] Part 1
    - [ ] Part 2
    - [ ] ...
- [ ] Build a Prototipe
- [ ] Power MSP432 thought USB 

## Testing
- [ ] Test all keys
- [ ] Test layers
- [ ] Test Led

# Final Product
- [ ] PCB
    - [ ] Add the power from thew USB
- [ ] Build PCB
- [ ] Final Testing
- [ ] Readme

# Delivery
- [ ] Pitch video
- [ ] Presentation
