#include "defs.h"
#include "uart.h"

#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>


String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup()
{
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(100);
    while (!Serial);
    uartInit();
    inputString.reserve(256);
    Serial.println("Setup complete");
}


uint8_t getTankNumber(const String &input)
{
    int start = input.indexOf('(');
    if (start < 0)
        return -1;

    int end = input.indexOf(')');

    String substr = input.substring(start+1, end);
    return (uint8_t)substr.toInt();
}


float getMassValue(const String &input)
{
    int start = input.indexOf(':');
    if (start < 0)
        return -1.0;

    String substr = input.substring(start+1);
    return (float)atof(substr.c_str());
}


void loop()
{
    if (stringComplete) {
        /*
         * Formats:
         *
         * t(2) -- tare tank 2
         * c1(2):69.01 -- confirm mass 1 for tank 2 as 69.01 kg
         * c2(2):42.14 -- confirm mass 2 as 42.14 kg
         * c3(2) -- end calibration of tank 2
         * r! -- reset arduino
         */

        Command command;

        switch (inputString.charAt(0)) {
            case 't': {
                // tare
                uint8_t tareTank = getTankNumber(inputString);
                if (tareTank < 0) {
                    Serial.println("INVALID TANK");
                    break;
                }
                Serial.println("TARING TANK " + String(tareTank));
                command.type = Tare;
                command.ttl = tareTank;
                break;
            }
            case 'c': {
                // calibration
                uint8_t zeroTank = getTankNumber(inputString);
                if (zeroTank < 0) {
                    Serial.println("INVALID TANK");
                    break;
                }
                Serial.println("CALIBRATING TANK " + String(zeroTank));
                command.ttl = zeroTank;
                command.type = Calibrate;
                switch (inputString.charAt(1)) {
                    case '1': {
                        // confirm mass 1
                        command.step = 1;
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        command.data = mass;
                        break;
                    }
                    case '2': {
                        // confirm mass 2
                        command.step = 2;
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        command.data = mass;
                        break;
                    }
                    case '3': {
                        // getVoltageToMassFactor();
                        Serial.println("FINISHED CALIBRATING TANK " + String(zeroTank));
                        command.step = 3;
                        break;
                    }
                    default: {
                        // invalid
                        Serial.println("UNRECOGNIZED...");
                        break;
                    }
                }
                break;
            }
            case 'r': {
                // reset
                command.type = Reset;
                // But don't do anything with it...
                if (inputString.charAt(1) == '!') {
                    // TODO Implement software reset
                }
                break;
            }
            default: {
                // invalid
                Serial.println("INVALID COMMAND");
                break;
            }
        }
        uartSend(Up, &command);
        // clear the string:
        inputString = "";
        stringComplete = false;
    }

    if (uartReceive(Up)) {
        Update update;
        uartGetMessage(Up, &update);
        Serial.println(uartSerializeUpdate(&update));
    }
}


void serialEvent()
{
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        inputString += inChar;
        if (inChar == '\n') {
            stringComplete = true;
        }
    }
}
