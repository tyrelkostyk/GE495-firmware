// main.c
// Contains the entrypoint for the software including setup and the main loop.

#include "defs.h"
#include "uart.h"

#include <Arduino.h>

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

    Serial.println("ARDUINO: Started setup");
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();
    prevSampleTime = millis();
    // canInit();
    uartInit();

    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(MUX_PIN0, OUTPUT);
    pinMode(MUX_PIN1, OUTPUT);
    Serial.println("\nSetup Started");

    doADCPowerUpSequence();
    setADCSpeed(0);
    Serial.println("Starting initial tare");
    delay(300);
    tareAllLoadCells();
    Serial.println("Done initial tare");
    Serial.setTimeout(5000);
    Serial.println("Input mass 1:");
    String m1 = Serial.readStringUntil('\n');
    if (m1.length()) {
        Serial.println(m1.toDouble());
        getCalMass1(m1.toDouble());
    } else {
        Serial.println("0");
        getCalMass1(0);
    }
    while (Serial.available() > 1) Serial.read();

    delay(500);
    Serial.println("Input mass 2:");
    String m2 = Serial.readStringUntil('\n');
    if (m2.length()) {
        Serial.println(m2.toDouble());
        getCalMass2(m2.toDouble());
    } else {
        Serial.println("0");
        getCalMass2(0);
    }
    while (Serial.available() > 1) Serial.read();
    delay(500);
    Serial.setTimeout(250);

    getVoltageToMassFactor(mass1, voltage1, mass2, voltage2);
    Serial.println(voltageToMassFactor);
    Serial.println("\nSetup Complete");

    digitalWrite(LED_BUILTIN, HIGH);
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    int32_t data = getNMeasurements(5);

    Serial.print("Data = ");
    Serial.println(data * voltageToMassFactor);

    if (millis() - prevUpdateTime > UPDATE_DELAY_MS) {
        prevUpdateTime = millis();
//        updateLoadCurrentData(data * voltageToMassFactor);
//        updateSendDownstream();
        Update msg;
        msg.tank = 1;
        msg.data = data * voltageToMassFactor;
        uartSend(Down, &msg);
    }

    Command command;
    Update update;

    if (uartReceive(Up)) {
        uartGetMessage(Up, &update);
    }
    if (uartReceive(Down)) {
        uartGetMessage(Down, &command);
    }
}
