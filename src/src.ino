#include <avr/wdt.h>

#include "defs.h"


String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int32_t dataOffset0 = 0;
int32_t dataOffset1 = 0;
int32_t dataOffset2 = 0;

double voltageToMassFactor = 1;
double mass1 = 0;
double mass2 = 1;
int32_t voltage1 = 0;
int32_t voltage2 = 1;

void setup()
{
    Serial.begin(SER_BAUDRATE);
    inputString.reserve(256);
    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(MUX_PIN0, OUTPUT);
    pinMode(MUX_PIN1, OUTPUT);
    doADCPowerUpSequence();
    setADCSpeed(0);
    delay(300);
    tareAllLoadCells();
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
    int32_t data = getNMeasurements(5);
    Serial.print("Data = ");
    Serial.println(data * voltageToMassFactor);
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

        switch (inputString.charAt(0)) {
            case 't': {
                // tare
                uint8_t tareTank = getTankNumber(inputString);
                if (tareTank < 0) {
                    Serial.println("INVALID TANK");
                    break;
                }
                Serial.println("TARING TANK " + String(tareTank));
                // TODO Construct tare command and transmit over CAN
                uint32_t messageID = cmdConstructID(tareTank, PGN_TARE);
                message_t tareMessage = {
                        .id = messageID,
                        .length = 8,
                        .data = { 0 }
                };
                cmdSendUpstream(&tareMessage);
                break;
            }
            case 'c': {
                // calibration
                uint8_t zeroTank = getTankNumber(inputString);
                if (zeroTank < 0) {
                    Serial.println("INVALID TANK");
                    break;
                }
                switch (inputString.charAt(1)) {
                    case '1': {
                        // confirm mass 1
                        // TODO pack it manually
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        uint32_t messageID = cmdConstructID(zeroTank, PGN_CALIBRATE & PGN_CAL_CONF_M1);
                        message_t calibrateMessage = {
                                .id = messageID,
                                .length = 8,
                                .data = { 0 }
                        };
                        cmdSendUpstream(&calibrateMessage);
                        break;
                    }
                    case '2': {
                        // confirm mass 2
                        // TODO pack it manually
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        uint32_t messageID = cmdConstructID(zeroTank, PGN_CALIBRATE & PGN_CAL_CONF_M2);
                        message_t calibrateMessage = {
                                .id = messageID,
                                .length = 8,
                                .data = { 0 }
                        };
                        cmdSendUpstream(&calibrateMessage);
                        break;
                    }
                    case '3': {
                        // getVoltageToMassFactor();
                        uint32_t messageID = cmdConstructID(zeroTank, PGN_CALIBRATE & PGN_CAL_FINISH);
                        message_t calibrateMessage = {
                                .id = messageID,
                                .length = 8,
                                .data = { 0 }
                        };
                        cmdSendUpstream(&calibrateMessage);
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
