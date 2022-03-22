// debug.c
// Contains functions for communicating with a debugging device via serial or JTAG.

#include "defs.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

#include <string.h>

/**
 * Sends a string message to the debugger, with NO newline appended.
 * @param message The string to be debug-printed
 */
void debugPrint(const char *message)
{
    Serial.print(message);
}

/**
 * Sends a hex-formatted number to the debugger, with NO newline appended.
 * @param num The number to be displayed
 */
void debugPrintNumber(uint32_t num)
{
    Serial.print(num, HEX);
}

/**
 * Sends a string message to the debugger, with a newline appended.
 * @param message The string to be debug-printed
 */
void debugPrintLine(const char *message)
{
    Serial.println(message);
}

/**
 * Reads a newline-terminated string from the debugger.
 * @param char *buffer The buffer to read the serial text stream into.
 * @return 0 in the event of a serial timeout, number of characters read otherwise.
 */
uint32_t debugReadLine(char *buffer)
{
    return Serial.readBytesUntil('\n', buffer, DEBUG_INPUT_LEN_MAX);
}

/**
 * Scans the serial line for input and parses it.
 * @param command A command instance (deprecated?)
 */
void debugScan(message_t *command)
{
    char buffer[DEBUG_INPUT_LEN_MAX+1];
    if (!debugReadLine(buffer))
        return;
    
    if (strncmp(buffer, "1", 1) == 0) {
        debugHandshake(command);
    } else {
        debugPrintLine("Unrecognized debug command!");
    }
}

