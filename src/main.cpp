#include "defs.h"

// main.c
// Contains the entrypoint for the software including setup and the main loop.

// Leave undefined in the compiler if not compiling to an Arduino target


#ifdef _ARDUINO
#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

extern SoftwareSerial serUp;
extern AltSoftSerial serDown;



#endif  // _ARDUINO

// The last command received from downstream
message_t currentCommand;

// The last update received from upstream
message_t currentUpdate;

uint8_t mux;

int32_t dataOffset0 = 0;
int32_t dataOffset1 = 0;
int32_t dataOffset2 = 0;

double voltageToMassFactor = 1;
double mass1;
double mass2;
int32_t voltage1;
int32_t voltage2;

uint8_t offsetFlag = 0;


#ifdef Arduino_h
// Tracker for time elapsed on a given SoftwareSerial port
uint32_t prevUpdateTime;
#endif  // Arduino_h

// Initialization steps go in here
void setup()
{

    pinMode(LED_BUILTIN, OUTPUT);

#ifdef Arduino_h
    debugPrintLine("ARDUINO: Started setup");
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();

    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(MUX_PIN0, OUTPUT);
    pinMode(MUX_PIN1, OUTPUT);
    Serial.println("\nSetup Started");

    doADCPowerUpSequence();
    setADCSpeed(0);
    delay(300);
    tareAllLoadCells();
    Serial.println("Input mass 1:");
    while (Serial.available() == 0){}
    getCalMass1(Serial.parseInt());
    while (Serial.available() == 1){}

    Serial.println("Input mass 2:");
    while (Serial.available() == 0){}
    getCalMass2(Serial.parseInt());

    getVoltageToMassFactor(mass1, voltage1, mass2, voltage2);
    Serial.println(voltageToMassFactor);
    Serial.println("\nSetup Complete");



//    if (CANSetup() != OK)
//        debugPrintLine("ARDUINO: Setup encountered a recoverable issue");
//    else
//        debugPrintLine("ARDUINO: Completed setup");
#endif  // Arduino_h
}
// Repeated routines (e.g. comms polling) go in here
void loop()
{
  int32_t data = getNMeasurements(5);

  Serial.print("Data = ");
  Serial.println(data * voltageToMassFactor);
      
//#ifdef Arduino_h
//
//#ifdef _DBG
//    // In debug mode, wait for specific test inputs
//    debugScan(&currentCommand);
//    debugPrintLine("ARDUINO: Debug scan");
//#endif  // _DBG
//
//    if (millis() - prevUpdateTime > UPDATE_DELAY_MS) {
//        prevUpdateTime = millis();
//        updateLoadCurrentData(&currentUpdate);
//        updateSendDownstream(&currentUpdate);
//    }
//    
//    // Poll for commands and respond accordingly
//    // If a command is received from downstream (ECU-side) immediately forward upstream
//    // Then check to see if the command requires any action from this device
//    if (cmdReceiveDownstream(&currentCommand)) {
//        cmdSendUpstream(&currentCommand);
//        cmdParse(&currentCommand);
//    }
//
//    // Poll for updates and respond accordingly
//    // If an update is received from upstream, handle (e.g. provide modifications)
//    // Then forward the modified message downstream
//    if (updateReceiveUpstream(&currentUpdate)) {
//        updateHandle(&currentUpdate);
//        updateSendDownstream(&currentUpdate);
//    }
//
//
//
//#endif  // Arduino_h
}

// It's important that main be wrapped in this, otherwise it interferes with
// the way the Arduino processor runs the loop
#ifndef Arduino_h

int main(void)
{

    setup();

    while (1) {
        loop();
    }
    
    return 0;
}

#endif  // Arduino_h
