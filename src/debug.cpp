// debug.c
// Contains functions for communicating with a debugging device via serial or JTAG.

#ifdef _ARDUINO
#include <SoftwareSerial.h>
#endif

#include <string.h>

#include "defs.h"

/**
 * Sends a string message to the debugger, with NO newline appended.
 * @param char *message The string to be debug-printed.
 * @return void
 */
void debugPrint(const char *message)
{
#ifdef Arduino_h
    Serial.print(message);
#endif
}

/**
 * Sends a string message to the debugger, with a newline appended.
 * @param char *message The string to be debug-printed.
 * @return void
 */
void debugPrintLine(const char *message)
{
#ifdef Arduino_h
    Serial.println(message);
#endif
}

/**
 * Reads a newline-terminated string from the debugger.
 * @param char *buffer The buffer to read the serial text stream into.
 * @return 0 in the event of a serial timeout, number of characters read otherwise.
 */
uint32_t debugReadLine(char *buffer)
{
#ifdef Arduino_h
    return Serial.readBytesUntil('\n', buffer, DEBUG_INPUT_LEN_MAX);
#endif
}
