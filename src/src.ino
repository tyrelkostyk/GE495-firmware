#include <avr/wdt.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


void setup()
{
    Serial.begin(9600);
    inputString.reserve(256);
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
    return substr.toFloat();
}


void loop()
{
    if (stringComplete) {
        /*
         * Formats:
         *
         * T(2) -- tare tank 2
         * Z0(2) -- start zero of tank 2
         * Z1(2):69.01 -- confirm mass 1 for tank 2 as 69.01 kg
         * Z2(2):42.14 -- confirm mass 2 as 42.14 kg and end zero for tank 2
         * R! -- reset arduino
         */
        Serial.println(inputString);
        switch (inputString.charAt(0)) {
            case 'T': {
                // tare
                uint8_t tareTank = getTankNumber(inputString);
                if (tareTank < 0) {
                    Serial.println("INVALID TANK #...");
                    break;
                }
                Serial.println("TARING TANK #" + String(tareTank) + "...");
                break;
            }
            case 'Z': {
                // zero
                uint8_t zeroTank = getTankNumber(inputString);
                if (zeroTank < 0) {
                    Serial.println("INVALID TANK #...");
                }
                switch (inputString.charAt(1)) {
                    case '0': {
                        // start
                        Serial.println("START TANK #" + String(zeroTank) + " ZERO...");
                        break;
                    }
                    case '1': {
                        // confirm mass 1
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        Serial.println("TANK #" + String(zeroTank) + " MASS 1 " + String(mass));
                        break;
                    }
                    case '2': {
                        // confirm mass 2
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        Serial.println("TANK #" + String(zeroTank) + " MASS 2 " + String(mass));
                        Serial.println("ZEROING COMPLETE!");
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
            case 'R': {
                // reset
                if (inputString.charAt(1) == '!') {
                    Serial.println("RESETTING...");
                }
                break;
            }
            default: {
                // invalid
                Serial.println("INVALID...");
                break;
            }
        }
        // clear the string:
        inputString = "";
        stringComplete = false;
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