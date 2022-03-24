// main.c
// Contains the entrypoint for the software including setup and the main loop.

#include "defs.h"
#include "uart.h"

#include <Arduino.h>


// Tracker for time elapsed on a given SoftwareSerial port
uint32_t prevUpdateTime;


void processCommand(Command *command)
{
    if (command->ttl == 0) {
        // handle it here
        switch (command->type) {
            case Tare:
                tareAllLoadCells();
                break;

            case Calibrate:
                switch (command->step) {
                    case 1:
                        calibrateMass1(command->data);
                        break;
                    case 2:
                        calibrateMass2(command->data);
                        calculateVoltageToMassFactor();
                        break;
                    case 3:
                        calculateVoltageToMassFactor();
                        break;
                    default:
                        break;
                }
                break;

            case Reset:

                break;

            default:
                break;
        }
    } else {
        --command->ttl;
        uartSend(Up, command);
    }
}


void processUpdate(Update *update)
{
    ++(update->tank);
    uartSend(Down, update);
}


// Initialization steps go in here
void setup()
{

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("ARDUINO: Started setup");
    Serial.begin(SER_BAUDRATE);
    Serial.setTimeout(250);
    while (!Serial);
    prevUpdateTime = millis();
    uartInit();

    pinMode(DATA_PIN, INPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(MUX_PIN0, OUTPUT);
    pinMode(MUX_PIN1, OUTPUT);


    doADCPowerUpSequence();
//    setADCSpeed(0); // ADC speed is hardwired to fast (tied high)
    Serial.println("Starting initial tare");
    delay(250);
    tareAllLoadCells();
    Serial.println("Done initial tare");

    delay(500);

    calibrateMass1(0);
    calibrateMass2(1);
    calculateVoltageToMassFactor();
    Serial.println("\nSetup Complete");

    digitalWrite(LED_BUILTIN, HIGH);
}

// Repeated routines (e.g. comms polling) go in here
void loop()
{
    digitalWrite(LED_BUILTIN, HIGH);
    int32_t data = getNMeasurements(SAMPLE_SIZE);

    if (millis() - prevUpdateTime > UPDATE_DELAY_MS) {
        prevUpdateTime = millis();
//        updateLoadCurrentData(data * voltageToMassFactor);
//        updateSendDownstream();
        Update msg;
        msg.tank = 0;
        msg.data = getCalibratedMassReading(data);
        uartSend(Down, &msg);
    }


    if (uartReceive(Up)) {
        Update update;
        uartGetMessage(Up, &update);
        processUpdate(&update);
    }
    if (uartReceive(Down)) {
        Command command;
        uartGetMessage(Down, &command);
        processCommand(&command);
    }
}
