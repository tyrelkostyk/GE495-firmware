# GE495 Firmware
**TBD: The Backstreet Digitizers**

## What is this project?
This project contains the firmware for a specific digitizer prototype to be used in Bourgault air seeder carts, converting analog load cell signals into digital representations to be transmitted to a tractor's ECU.

## Branches
There are thee primary branches: `sam4e16-xplained`, `sam4e8e`, and `arduino`.

The `sam4e16-xplained` branch is the trunk for all development code that will be run on the **ATSAM4E16** evaluation kit.

The `sam4e8e` branch is the trunk for all production code that will be run on the **ATSAM4E8E** microcontroller at the heart of the digitizer.

Note that the `main` branch should generally not be used.

The `arduino` branch is the trunk for all "model" code used in early prototyping and configuration validation. The Arduino setup represents a proof-of-concept for the idea on a large scale without getting too deep into the specifics of low-level device code.

## Contributing
1. Download and install [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio). In the installation process select the AVR and SAM architectures.
2. Clone this repository (`git clone git@github.com:tyrelkostyk/GE495-firmware`).
3. Make a working branch off of `main` (or a secondary trunk if one exists).
4. Open Microchip Studio and create a new project solution.
5. Enter the project details in the dialog:
    - Select the **GCC C ASF Board Project** type
    - Give the project a name (name is unimportant)
    - Set the location to the root of the local Git repository for the project
    - Select **Create new solution** and **deselect** Create directory for solution
    - Press OK
6. In the Board Selection dialog, search for and select **ATSAM4E8E** in the list, then press OK.
7. After the project and solution have generated, close the solution using the File menu, browse to the repository folder, and **move** (or cut-paste) all of the contents of the `<project-name>` directory to the root repository folder. The result should be a merging of the two `src` folders, and the `.atsln` and `.cproj` and `.xml` files should all be in the root folder, along with an empty folder named `Debug`.
8. In Microchip Studio, browse for and open the newly-moved solution file `<project-name>.atsln`.
9. In the ASF menu (a few spaces to the right of the File menu), open the ASF Wizard. Ensure that the correct project is selected in the dropdown. Refer to [MODULES.md](MODULES.md) for an up-to-date list of which modules to select. Select each required module one at a time from the left column (don't expand them with the arrows) and press **Add**. When finished, press **Apply** and close the Wizard.
10. Done!

## Authors
- [Tyrel Kostyk](https://github.com/tyrelkostyk)
- [Jeremy Rempel](https://github.com/jdrempel)
- Thomas Slotboom

