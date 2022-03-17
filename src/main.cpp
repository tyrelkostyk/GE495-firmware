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
void _setup()
{
    Serial.begin(19200);
    Serial.setTimeout(500);
    while (!Serial);
}

// Repeated routines (e.g. comms polling) go in here
void _loop()
{
    // Poll for commands and respond accordingly
    // If a command is received from downstream (ECU-side) immediately forward upstream
    // Then check to see if the command requires any action from this device
    while (!Serial.available());
    if (Serial.available()) {
        Serial.println("hello");
        if (cmdReceiveDownstream(&currentCommand)) {
            cmdSendUpstream(&currentCommand);
            cmdParse(&currentCommand);
        }
        Serial.println("Bye for now");
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

/**
 * Converts a float into an integer representation of a float
 * @param float f the float to be converted
 * @param bits the number of bits
 * @param expbits the number of exponent bits
 * @return an integer
 */
 uint32_t packFloat754(float f, uint8_t bits, uint8_t expbits)
 {
  float fnorm;
  uint32_t shift;
  uint32_t sign, exp, significand;
  uint32_t significandbits = bits - expbits - 1;

  if (f == 0.0) return 0;

  if (f < 0) {sign = 1; fnorm = -f; }
  else {sign = 0; fnorm = f; }

  shift = 0;
  while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
  while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
  fnorm = fnorm - 1.0;
  significand = fnorm * ((1LL<<significandbits) + 0.5f);

  exp = shift + ((1<<(expbits-1)) - 1);

  return (sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand;
 }
