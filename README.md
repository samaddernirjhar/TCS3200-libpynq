# TCS3200 colour Sensor Interface

## Overview

This project interfaces with a TCS3200 colour sensor using an AMD XILINX PynQ Z2 board. The code configures the sensor, reads the colour frequencies, and determines the detected colour (Red, Green, Blue, or Clear). It continuously outputs the colour data to the terminal.

## Requirements

- **Hardware:**
  - PYNQ-Z2
  - TCS3200 colour sensor
  - Connecting wires

- **Software:**
  - PYNQ framework
  - GPIO library compatible with PYNQ (libpynq)
  - Standard C libraries: `<stdio.h>`, `<time.h>`

## Pin Configuration

The TCS3200 colour sensor is connected to the PYNQ board with the following pin configuration, but you can change it should you wish so, and it will work just fine:

| Sensor Pin | PYNQ Pin |
|------------|-----------|
| OUT        | IO_AR4    |
| OE         | IO_AR5    |
| S0         | IO_AR0    |
| S1         | IO_AR1    |
| S2         | IO_AR2    |
| S3         | IO_AR3    |

## Code Functionality:
- Sets the direction of `OE`, `S0`, `S1`, `S2`, `S3` as outputs.
- Sets the direction of `OUT` as input.
- Configures the sensor to use 100% frequency scaling.
- Enables the sensor by setting `OE` to LOW.

### `pulseIn()`

This function measures the pulse width of a specified GPIO pin:
- Waits until the pin reaches the desired state.
- Measures the time the pin remains in that state.
- Returns the pulse width in microseconds.

## Usage

1. Connect the TCS3200 colour sensor to the PYNQ board using the desired pin configuration.
2. Compile and run the program on the PYNQ-enabled board.
3. Observe the terminal output to see the detected colour and corresponding frequencies.

The program will continuously detect and print the colour until it is manually stopped.

## Notes

- Ensure that the TCS3200 sensor and PYNQ board are properly powered and connected.
- Adjust the sleep interval in the `main()` function if necessary to fit your application's requirements.

