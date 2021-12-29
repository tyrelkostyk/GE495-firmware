// main.c
// Contains the entrypoint for the software including setup and the main loop.

// Leave undefined in the compiler if not compiling to an Arduino target
#ifdef _ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif  // _ARDUINO

#include "defs.h"

// The last command received from downstream
command_t currentCommand;

// Initialization steps go in here
void setup()
{
#ifdef Arduino_h
    Serial.begin(SER_BAUDRATE);
    CANSetup();
    Serial.println("ARDUINO: Completed setup");
#endif
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
#ifdef Arduino_h
    // Poll for commands and respond accordingly
    // If a command is received from downstream (ECU-side) immediately forward upstream
    // Then check to see if the command requires any action from this device
    if (cmdReceiveDownstream(&currentCommand)) {
        cmdSendUpstream(&currentCommand);
        cmdParse(&currentCommand);
    }
#endif
}

// It's important that main be wrapped in this, otherwise it interferes with
// the way the Arduino processor runs the loop
#ifndef Arduino_h

int main(void)
{

    setup();

    while (1) {
        loop();
    }
    
    return 0;
}

#endif  // Arduino_h
