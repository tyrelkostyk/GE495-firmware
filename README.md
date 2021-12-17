# GE495-firmware

_This repository contains the project files for the firmware to be used
in the GE 495 digitizer project for Bourgault Industries Ltd._

## Branches
- arduino: Contains the code written specifically for the Arduino validation model
- main: Contains working code that runs on the microprocessor of choice for the project

## Arduino Setup
1. Install the `arduino-cli` tool on your machine ([LINK](https://arduino.github.io/arduino-cli/0.19/installation/))
2. Run `arduino-cli config init` to generate a default configuration file
3. Clone this repository if you haven't already
4. Run `arduino-cli core update-index` to update the local cache of available platforms and libraries
5. Run `arduino-cli core install arduino:avr:nano` to install the platform core for the Nano
6. Connect the Arduino Nano to your machine via USB
7. From the root directory of the cloned repository, run `make arduino` to compile and upload the current version of the code to the device

## Main Contributors
- Tyrel Kostyk
- Jeremy Rempel
- Thomas Slotboom
