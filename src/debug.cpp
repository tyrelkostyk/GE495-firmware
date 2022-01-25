// debug.c
// Contains functions for communicating with a debugging device via serial or JTAG.

#ifdef _ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif

#include <string.h>

#include "defs.h"

extern message_t currentCommand;
extern message_t currentUpdate;

extern uint32_t currentMass;

/**
 * Sends a string message to the debugger, with NO newline appended.
 * @param char *message The string to be debug-printed.
 * @return void
 */
void debugPrint(const char *message)
{
#ifdef _DBG
#ifdef Arduino_h
    Serial.print(message);

#endif  // Arduino_h
#endif  // _DBG
}

void debugPrintNumber(uint32_t num)
{
#ifdef _DBG
#ifdef Arduino_h
    Serial.print(num, HEX);
#endif  // Arduino_h
#endif  // _DBG
}

/**
 * Sends a string message to the debugger, with a newline appended.
 * @param char *message The string to be debug-printed.
 * @return void
 */
void debugPrintLine(const char *message)
{
#ifdef _DBG
#ifdef Arduino_h
    Serial.println(message);
#endif  // Arduino_h
#endif  // _DBG
}

/**
 * Reads a newline-terminated string from the debugger.
 * @param char *buffer The buffer to read the serial text stream into.
 * @return 0 in the event of a serial timeout, number of characters read otherwise.
 */
uint32_t debugReadLine(char *buffer)
{
#ifdef _DBG
#ifdef Arduino_h
    return Serial.readBytesUntil('\n', buffer, DEBUG_INPUT_LEN_MAX);
#endif  // Arduino_h
#endif  // _DBG
}

/**
 * Scans the debug input line for a debug command and executes it if valid.
 * @return void
 */
void debugScan(void)
{
    char buffer[DEBUG_INPUT_LEN_MAX+1];
    if (!debugReadLine(buffer))
        return;
    
    if (strncmp(buffer, "1", 1) == 0) {
        debugHandshake();
    } else {
        debugPrintLine("Unrecognized debug command!");
    }
}

/**
 * Executes a test handshake from this device to the next upstream device.
 * @return void
 */
void debugHandshake(void)
{
    currentCommand.id = 1<<8;
    currentCommand.length = CAN_DATA_LEN_MAX;
    memset(currentCommand.data, 2, CAN_DATA_LEN_MAX);  // Just to be safe

    cmdSendUpstream(&currentCommand);
}

