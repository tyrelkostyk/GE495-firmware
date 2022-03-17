// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#define _ARDUINO

#define OK  0x01
#define ERR 0x00
#define NOP 0xff

uint16_t crc16MCRFXX(uint16_t crc, uint8_t *data, uint8_t len);

float unpackFloat754(uint32_t i, uint8_t bits, uint8_t expbits);
uint32_t packFloat754(float f, uint8_t bits, uint8_t expbits);

#define unpackFloat(i) (unpackFloat754((f), 32, 8))

/******
* CAN *
******/

#define CAN_BAUDRATE (9600)
#define CAN_UP_TX (4)
#define CAN_UP_RX (5)

// Downstream CAN for Arduino now uses AltSoftSerial which requires RX 8, TX 9

#define CAN_FRAME_STD (0x00)
#define CAN_FRAME_EXT (0x01)

#define CAN_DATA_LEN_MAX (8)

// Enum for determining which CAN unit is desired (upstream or downstream)
typedef enum {
    UP,
    DOWN
} can_dir_t;

typedef struct _message_t {
    uint32_t id;
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX];
} message_t;

uint8_t CANSetup (void);
uint8_t CANSend (can_dir_t direction, uint32_t *id, uint8_t ext, uint8_t rtr,
                 uint8_t length, uint8_t **data);
uint8_t CANReceive (can_dir_t direction, uint32_t *id, uint8_t **buffer);

/*********
* SERIAL *
*********/

#define SER_BAUDRATE (9600)

/*
 * Telemetry:
 * uint8_t  tankID;
 * uint32_t mass;
 * uint16_t crc;
 * uint8_t  null;
*/
#define SER_TEL_SZ (8)

#define SER_CMD_SZ (64)

/****************
* COMMAND (CMD) *
****************/

uint8_t cmdSendUpstream (message_t *command);
uint8_t cmdReceiveDownstream (message_t *command);
uint8_t cmdParse (message_t *command);

// TODO These will definitely have to change (or will they?)

#define PGN_DEBUG_HANDSHAKE (0x01)

#define PGN_TARE (0x20)

#define PGN_CALIBRATE   (0x10)
#define PGN_CAL_CONF_M1 (0x01)
#define PGN_CAL_CONF_M2 (0x02)
#define PGN_CAL_FINISH  (0x04)

/*********
* UPDATE *
*********/

// TODO Deprecated?
#define UPDATE_MASS_LEN_MAX 8
#define UPDATE_DELAY_MS 1000

uint8_t updateReceiveUpstream(message_t *update);
uint8_t updateSendDownstream(message_t *update);
uint8_t updateHandle(message_t *update);

/*******
* MASS *
*******/

#define MASS_NUM_BYTES 4

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

#define NUM_ADC_BITS 24
#define NUM_LOAD_CELLS 3

/**************
* CALIBRATION *
**************/

void tare(uint8_t mux, int32_t offset);
void tareAllLoadCells();
void getCalMass1(double mass);
void getCalMass2(double mass);
void getVoltageToMassFactor();

/**********
* ARDUINO *
**********/


#define DATA_PIN 12
#define CLOCK_PIN 11
#define POWER_PIN 10

#define MUX_PIN0 4
#define MUX_PIN1 5

#define SPEED_PIN 7



#endif  // DEFS_H
