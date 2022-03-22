// main.c
// Contains the entrypoint for the software including setup and the main loop.

#include "defs.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

extern SoftwareSerial serUp;
extern AltSoftSerial serDown;

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


// Tracker for time elapsed on a given SoftwareSerial port
uint32_t prevUpdateTime;
uint32_t prevSampleTime;

// Initialization steps go in here
void setup()
{

    pinMode(LED_BUILTIN, OUTPUT);

    debugPrintLine("ARDUINO: Started setup");
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();
    prevSampleTime = millis();
    canInit();

    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(MUX_PIN0, OUTPUT);
    pinMode(MUX_PIN1, OUTPUT);
    Serial.println("\nSetup Started");

    // doADCPowerUpSequence();
    // setADCSpeed(0);
    // delay(300);
    // tareAllLoadCells();
    // Serial.println("Input mass 1:");
    // while (Serial.available() == 0){}
    // getCalMass1(Serial.parseInt());
    // while (Serial.available() == 1){}

    // Serial.println("Input mass 2:");
    // while (Serial.available() == 0){}
    // getCalMass2(Serial.parseInt());

    // getVoltageToMassFactor(mass1, voltage1, mass2, voltage2);
    // Serial.println(voltageToMassFactor);
    Serial.println("\nSetup Complete");

}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
    // int32_t data = getNMeasurements(5);

    // Serial.print("Data = ");
    // Serial.println(data * voltageToMassFactor);

    if (millis() - prevUpdateTime > UPDATE_DELAY_MS) {
        prevUpdateTime = millis();
        updateLoadCurrentData(&currentUpdate);
        updateSendDownstream(&currentUpdate);
    }

    // Poll for commands and respond accordingly
    // If a command is received from downstream (ECU-side) immediately forward upstream
    // Then check to see if the command requires any action from this device
    if (cmdReceiveDownstream() == OK) {
        cmdSendUpstream();
        cmdParse();
    }

    // Poll for updates and respond accordingly
    // If an update is received from upstream, handle (e.g. provide modifications)
    // Then forward the modified message downstream
    if (updateReceiveUpstream(&currentUpdate) == OK) {
        updateHandle(&currentUpdate);
        updateSendDownstream(&currentUpdate);
    }
}
