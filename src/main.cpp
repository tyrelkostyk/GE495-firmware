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
    Serial.setTimeout(100);
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

/**
 * Converts an integer representation of a float into an actual float
 * @param i the integer representation
 * @param bits the number of bits
 * @param expbits the number of exponent bits
 * @return a float
 */
float unpackFloat754(uint32_t i, uint8_t bits, uint8_t expbits)
{
    float result;
    long long shift;
    uint16_t bias;
    uint16_t significandBits = bits - expbits - 1;

    if (i == 0) return 0.0;

    result = (i & ((1LL << significandBits)-1));
    result /= (1LL << significandBits);
    result += 1.0;

    bias = (1 << (expbits - 1)) - 1;
    shift = ((i >> significandBits) & ((1LL << expbits)-1)) - bias;
    while (shift > 0) { result *= 2.0; shift--; }
    while (shift < 0) { result /= 2.0; shift++; }

    result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;

    return result;
}

