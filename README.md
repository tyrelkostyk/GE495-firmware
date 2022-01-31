# GE495 Firmware
**TBD: The Backstreet Digitizers**

## What is this project?
This project contains the firmware for a specific digitizer prototype to be used in Bourgault air seeder carts, converting analog load cell signals into digital representations to be transmitted to a tractor's ECU.

## Branches
There are two primary branches: `main` and `arduino`.

The `main` branch is the trunk for all "production" code that will be run on the **ATSAM4E8E** microcontroller at the heart of the digitizer.

The `arduino` branch is the trunk for all "model" code used in early prototyping and configuration validation. The Arduino setup represents a proof-of-concept for the idea on a large scale without getting too deep into the specifics of low-level device code.

## Contributing
1. Download and install [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio). In the installation process select the AVR and SAM architectures.
2. Clone this repository (`git clone git@github.com:tyrelkostyk/GE495-firmware`).
3. Make a working branch off of `main` (or a secondary trunk if one exists).
4. Open the project solution in Microchip Studio.

## Authors
- [Tyrel Kostyk](https://github.com/tyrelkostyk)
- [Jeremy Rempel](https://github.com/jdrempel)
- Thomas Slotboom

