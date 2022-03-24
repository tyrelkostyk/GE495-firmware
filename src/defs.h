// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#define OK  (0x01)
#define ERR (0x00)
#define NOP (0xff)

// Enum for determining which CAN unit is desired (upstream or downstream)
typedef enum {
    Up,
    Down
} can_dir_t;

#define CAN_UP_RX (4)
#define CAN_UP_TX (5)

typedef can_dir_t dir_t;

/*********
* SERIAL *
*********/

#define SER_BAUDRATE 19200
#define UART_BAUDRATE 19200

struct Message;
struct Command;
struct Update;

String uartSerializeUpdate(Update *update);
void uartInit(void);
uint8_t uartSend(dir_t dir, Message *message);
bool uartReceive(dir_t dir);
void uartGetMessage(dir_t dir, Message *message);

/****************
* COMMAND (CMD) *
****************/

typedef enum {
    Tare,
    Calibrate,
    Reset,
    Invalid
} cmd_type_t;

/*********
* UPDATE *
*********/

#define UPDATE_DELAY_MS (1000)

/*******
* MASS *
*******/

#define SAMPLE_SIZE  (5)

extern double mass1;
extern double mass2;
extern int32_t voltage1;
extern int32_t voltage2;
extern double voltageToMassFactor;

/******
* I2C *
******/

void applySCLK();
void setSCLKHigh();
void setSCLKLow();

/******
* ADC *
******/

extern int32_t dataOffset0;
extern int32_t dataOffset1;
extern int32_t dataOffset2;

int32_t retrieveADCData();
int32_t retrieveADCDataWithCal();
void setADCMux(uint8_t muxSelect);
void doADCPowerUpSequence();
void setADCSpeed(uint8_t sampleSpeed);
void blockingWaitForData();
int32_t readThreeLoadCells();
int32_t getNMeasurements(int32_t N);
int32_t getNRawMeasurements(uint8_t mux, int32_t N);

#define NUM_ADC_BITS   (24)
#define NUM_LOAD_CELLS (3)

/**************
* CALIBRATION *
**************/

void tare(uint8_t mux, int32_t offset);
void tareAllLoadCells();
void getCalMass1(double mass);
void getCalMass2(double mass);
void getVoltageToMassFactor(double mass1, int32_t voltage1, double mass2, int32_t voltage2);

/**********
* ARDUINO *
**********/

#define DATA_PIN  (12)
#define CLOCK_PIN (11)
#define POWER_PIN (10)

#define MUX_PIN0 (2)
#define MUX_PIN1 (3)

#define SPEED_PIN (7)

#endif  // DEFS_H
