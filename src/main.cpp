// main.c
// Contains the entrypoint for the software including setup and the main loop.

// Leave undefined in the compiler if not compiling to an Arduino target
#ifdef _ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif  // _ARDUINO

#include "defs.h"

// The last command received from downstream
message_t currentCommand;

// The last update received from upstream
message_t currentUpdate;

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

#ifdef _DEBUG
    // In debug mode, wait for specific test inputs
    debugScan();
#endif  // _DEBUG
    
    // Poll for commands and respond accordingly
    // If a command is received from downstream (ECU-side) immediately forward upstream
    // Then check to see if the command requires any action from this device
    if (cmdReceiveDownstream(&currentCommand)) {
        cmdSendUpstream(&currentCommand);
        cmdParse(&currentCommand);
    }

    // Poll for updates and respond accordingly
    // If an update is received from upstream, handle (e.g. provide modifications)
    // Then forward the modified message downstream
    if (updateReceiveUpstream(&currentUpdate)) {
        updateHandle(&currentUpdate);
        updateSendDownstream(&currentUpdate);
    }

#endif  // Arduino_h
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
