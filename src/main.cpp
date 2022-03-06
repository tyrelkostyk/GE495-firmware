// main.cpp
// Contains the entrypoint for the software including setup and the main loop.

#include "defs.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

extern SoftwareSerial serUp;

// The last command received from downstream
message_t currentCommand;

// The last update received from upstream
message_t currentUpdate;

// Tracker for time elapsed on a given SoftwareSerial port
uint32_t prevUpdateTime;
uint32_t prevSampleTime;

// Initialization steps go in here
void setup()
{
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();
    prevSampleTime = millis();
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
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
}
