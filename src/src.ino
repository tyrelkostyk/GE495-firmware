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
  //    float mass1 = 0;
//    float mass2 = 5;
//    uint32_t mass1_int = packFloat754(mass1, 32, 8);
//    uint32_t mass2_int = packFloat754(mass2, 32, 8);
  
//    message_t tare = {
//      .id = (PGN_TARE) << 8,
//      .length = 8,
//      .data = {0,0,0,0, 0,0,0,0}
//    };
//
//    message_t cal_1 = {
//      .id = (PGN_CALIBRATE | PGN_CAL_CONF_M1) << 8,
//      .length = 8,
//      .data = {
//      (uint8_t)(mass1_int & 0x000000FF),
//      (uint8_t)(mass1_int & 0x0000FF00) >> 2,
//      (uint8_t)(mass1_int & 0x00FF0000) >> 4,
//      (uint8_t)(mass1_int & 0xFF000000) >> 6,
//      0,
//      0,
//      0,
//      0}
//    };
//
//    message_t cal_2 = {
//      .id = (PGN_CALIBRATE | PGN_CAL_CONF_M2) << 8,
//      .length = 8,
//      .data = {
//      (uint8_t)(mass2_int & 0x000000FF),
//      (uint8_t)(mass2_int & 0x0000FF00) >> 2,
//      (uint8_t)(mass2_int & 0x00FF0000) >> 4,
//      (uint8_t)(mass2_int & 0xFF000000) >> 6,
//      0,
//      0,
//      0,
//      0}
//    };
//
//    message_t cal_3 = {
//      .id = (PGN_CALIBRATE | PGN_CAL_FINISH) << 8,
//      .length = 0,
//      .data = {0,0,0,0, 0,0,0,0}
//    };

    doADCPowerUpSequence();
    setADCSpeed(0);
    delay(300);
    tareAllLoadCells();

//    Serial.println("Input mass 1:");
//    while (Serial.available() == 0){}
//    getCalMass1(Serial.parseInt());
//    while (Serial.available() == 1){}
//
//    Serial.println("Input mass 2:");
//    while (Serial.available() == 0){}
//    getCalMass2(Serial.parseInt());
//
//    getVoltageToMassFactor(mass1, voltage1, mass2, voltage2);
//    Serial.println(voltageToMassFactor);
//    Serial.println("\nSetup Complete");

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
         * T(2) -- tare tank 2
         * C1(2):69.01 -- confirm mass 1 for tank 2 as 69.01 kg
         * C2(2):42.14 -- confirm mass 2 as 42.14 kg
         * C3(2) -- end calibration of tank 2
         * R! -- reset arduino
         */

        Serial.println("Received message: " + inputString);
        switch (inputString.charAt(0)) {
            case 't': {
                // tare
//                uint8_t tareTank = getTankNumber(inputString);
//                if (tareTank < 0) {
//                    Serial.println("INVALID TANK #...");
//                    break;
//                }
//                Serial.println("TARING TANK #" + String(tareTank) + "...");
                tareAllLoadCells();
                Serial.println("TARING");
                break;
            }
            case 'c': {
                // calibration
//                uint8_t zeroTank = getTankNumber(inputString);
//                if (zeroTank < 0) {
//                    Serial.println("INVALID TANK #...");
//                }
                switch (inputString.charAt(1)) {
                    case '1': {
                        // confirm mass 1
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        getCalMass1(mass);
                        Serial.println("MASS 1 " + String(mass));
                        break;
                    }
                    case '2': {
                        // confirm mass 2
                        float mass = getMassValue(inputString);
                        if (mass < 0) {
                            Serial.println("INVALID MASS");
                            break;
                        }
                        getCalMass2(mass);
                        Serial.println("MASS 2 " + String(mass));
                        break;
                    }
                    case '3': {
                        getVoltageToMassFactor();                      
                        Serial.println("CAL STEP THREE RECEIVED");
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
//                if (inputString.charAt(1) == '!') {
                Serial.println("RESETTING...");
//                }
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
