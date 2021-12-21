// main.c
// Contains the entrypoint for the software including setup and the main loop.

// Leave undefined in the compiler if not compiling to an Arduino target
#ifdef ARDUINO
#include <Arduino.h>
#include <SoftwareSerial.h>
#endif  // ARDUINO

#include "defs.h"

// Initialization steps go in here
void setup()
{
#ifdef Arduino_h
    pinMode(LED_BUILTIN, OUTPUT);
#endif
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
#ifdef Arduino_h
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
#endif
}

// It's important that main be wrapped in this, otherwise it interferes with
// the way the Arduino processor runs the loop
#ifndef Arduino_h

int main(void)
{

    setup();

    while (1)
    {
        loop();
    }
    
    return 0;
}

#endif  // Arduino_h
