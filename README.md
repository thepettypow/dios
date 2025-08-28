# ESP32 LED Control Simulation

A C program that simulates controlling LEDs with push buttons on an ESP32 microcontroller. Since no hardware is available, this implementation uses mock GPIO registers and functions to represent pin states, providing a realistic simulation of embedded hardware control.

## Features

- **Mock GPIO System**: Simulates ESP32 GPIO registers and pin operations
- **LED Control**: Virtual LEDs that can be turned on/off and toggled
- **Button Input**: Simulated push buttons with debouncing
- **Modular Design**: Clean separation between hardware abstraction and application logic
- **Real-time Feedback**: Console output showing all GPIO operations and state changes
- **Interactive Testing**: Command-line interface for simulating button presses

## Project Structure

```
├── gpio_mock.h         # GPIO definitions and function declarations
├── gpio_mock.c         # Mock GPIO register simulation
├── led_control.h       # LED management interface
├── led_control.c       # LED control implementation
├── button_control.h    # Button handling interface
├── button_control.c    # Button control with debouncing
├── main.c              # Main application and control loop
├── Makefile            # Build configuration
└── README.md           # This file
```

## Hardware Mapping

| Component | GPIO Pin | Description |
|-----------|----------|-------------|
| LED1      | Pin 2    | First LED   |
| LED2      | Pin 4    | Second LED  |
| LED3      | Pin 5    | Third LED   |
| BTN1      | Pin 18   | First Button (controls LED1) |
| BTN2      | Pin 19   | Second Button (controls LED2) |
| BTN3      | Pin 21   | Third Button (controls LED3) |

## Building and Running

### Prerequisites
- GCC compiler
- Make utility
- Unix-like system (macOS, Linux)

### Build Commands

```bash
# Clean and build
make clean && make

# Build and run
make run

# Debug build
make debug

# Release build
make release

# Show all available targets
make help
```

### Running the Program

```bash
./esp32_led_sim
```

## Usage

Once the program is running, you can use these commands:

- `1`, `2`, `3` - Simulate button press on BTN1, BTN2, BTN3
- `r1`, `r2`, `r3` - Simulate button release on BTN1, BTN2, BTN3
- `s` - Show status of all LEDs and buttons
- `h` - Show help menu
- `q` - Quit program

### Example Session

```
[MAIN] Starting ESP32 LED Control Simulation...
[GPIO] Mock GPIO system initialized
[LED] Initializing LEDs...
[BUTTON] Initializing buttons...
[MAIN] System initialization complete!

=== ESP32 LED Control Simulation ===
Commands:
  1, 2, 3    - Simulate button press on BTN1, BTN2, BTN3
  ...

[MAIN] Entering main loop. Type 'h' for help, 'q' to quit.

1
[SIMULATION] Button on pin 18 pressed
[BUTTON] BTN1 PRESSED
[MAIN] Button 1 pressed - Toggling LED1
[GPIO] Pin 2 set to HIGH
[LED] LED1 turned ON
```

## Code Architecture

### GPIO Mock Layer (`gpio_mock.c/h`)
- Simulates ESP32 GPIO registers
- Provides `gpio_set_level()` and `gpio_get_level()` functions
- Tracks pin modes (input/output) and pull-up configuration
- Includes validation and error handling

### LED Control Layer (`led_control.c/h`)
- High-level LED management interface
- Functions for turning LEDs on/off, toggling, and status display
- Maintains LED state tracking
- Clean abstraction over GPIO operations

### Button Control Layer (`button_control.c/h`)
- Button input handling with debouncing
- Edge detection for press/release events
- Non-blocking button state updates
- Simulation functions for testing

### Main Application (`main.c`)
- System initialization and main control loop
- Event processing and LED control logic
- User interface for interactive testing
- Graceful shutdown handling

## Key Features

### Realistic Hardware Simulation
- Mock GPIO registers behave like real ESP32 hardware
- Proper pin configuration and validation
- Realistic timing and state management

### Debounced Button Input
- 50ms debounce delay to prevent false triggers
- Edge detection for press and release events
- State tracking with change notifications

### Modular Design
- Clear separation of concerns
- Hardware abstraction layer
- Easy to port to real ESP32 hardware
- Testable components

### Error Handling
- Input validation for all GPIO operations
- Proper error messages and logging
- Graceful degradation on invalid inputs

## Porting to Real Hardware

To port this code to real ESP32 hardware:

1. Replace `gpio_mock.c` with ESP-IDF GPIO functions
2. Update `#include` statements for ESP-IDF headers
3. Modify timing functions to use ESP-IDF APIs
4. Adjust pin definitions for your specific hardware setup

The modular design ensures that only the GPIO mock layer needs to be replaced, while the LED and button control logic remains unchanged.

## License

This project is provided as-is for educational and demonstration purposes.
But you can buy me a coffee if you see me :)