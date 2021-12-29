// debug.c
// Contains functions for communicating with a debugging device via serial or JTAG.

#ifdef _ARDUINO
#include <SoftwareSerial.h>
#endif

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

