// main.c
// Contains the entrypoint for the software including setup and the main loop.

#include "defs.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

extern SoftwareSerial serUp;
extern AltSoftSerial serDown;

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
    debugPrintLine("ARDUINO: Started setup");
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();
    prevSampleTime = millis();
    canInit();
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
    // In debug mode, wait for specific test inputs
    // debugScan(&currentCommand);

    if (millis() - prevUpdateTime > UPDATE_DELAY_MS) {
        prevUpdateTime = millis();
        updateLoadCurrentData(&currentUpdate);
        updateSendDownstream(&currentUpdate);
    }

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
