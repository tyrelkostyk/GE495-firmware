// main.c
// Contains the entrypoint for the software including setup and the main loop.

#include <Arduino.h>

#include "defs.h"

// Initialization steps go in here
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}

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
