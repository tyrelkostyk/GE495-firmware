// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

#define OK  (0x01)
#define ERR (0x00)
#define NOP (0xff)

typedef struct _message_t message_t;

/********
* DEBUG *
********/

#define DEBUG_INPUT_LEN_MAX (1<<7)

void debugPrint(const char *message);
void debugPrintLine(const char *message);
void debugPrintNumber(uint32_t num);
uint32_t debugReadLine(char *buffer);
void debugScan(message_t *command);
uint8_t debugHandshake(message_t *command);

/******
* CAN *
******/

#define CAN_BAUDRATE (9600)
#define CAN_UP_RX (4)
#define CAN_UP_TX (5)

// Downstream CAN for Arduino now uses AltSoftSerial which requires RX 8, TX 9
// #define CAN_DOWN_TX 6
// #define CAN_DOWN_RX 7

#define CAN_FRAME_STD (0x00)
#define CAN_FRAME_EXT (0x01)

#define CAN_DATA_LEN_MAX (8)

// Enum for determining which CAN unit is desired (upstream or downstream)
typedef enum {
    Up,
    Down
} can_dir_t;

typedef struct _message_t {
    uint32_t id;
    // uint8_t length;  // Unnecessary
    uint8_t data[CAN_DATA_LEN_MAX];
} message_t;

uint8_t canInit (void);
uint8_t canSend (can_dir_t direction, uint32_t id, uint8_t ext, uint8_t rtr,
                 uint8_t length, const uint8_t *data);
uint8_t canReceive (can_dir_t direction, uint32_t *id, uint8_t *buffer);

/*********
* SERIAL *
*********/

#define SER_BAUDRATE 115200

/****************
* COMMAND (CMD) *
****************/

uint8_t cmdSendUpstream (message_t *command);
uint8_t cmdReceiveDownstream (message_t *command);
uint8_t cmdParse (message_t *command);

#define PGN_POSITION    (8)
/*
 * PGN Format:
 * [28 ... 9   8   7   6   5   4   3   2   1   0  ]
 * [X... TARE|CAL|   CAL STEP    | X |  TANK  ID  ]
 */
#define PGN_TANK_IDX    (0)
#define PGN_CMD_IDX     (4)

#define PGN_DEBUG_HANDSHAKE 0x01

#define PGN_TARE (0x20)

#define PGN_CALIBRATE   (0x10)
#define PGN_CAL_CONF_M1 (1<<0)
#define PGN_CAL_CONF_M2 (1<<1)
#define PGN_CAL_FINISH  (1<<2)

/*********
* UPDATE *
*********/

// TODO Deprecated?
#define UPDATE_MASS_LEN_MAX (8)
#define UPDATE_DELAY_MS (1000)

uint8_t updateReceiveUpstream(message_t *update);
uint8_t updateSendDownstream(message_t *update);
uint8_t updateHandle(message_t *update);
uint8_t updateLoadCurrentData(message_t *update);

/*******
* MASS *
*******/

#define MASS_NUM_BYTES (4)

void massGetCurrent(uint8_t *buffer);

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

#define MUX_PIN0 (4)
#define MUX_PIN1 (5)

#define SPEED_PIN (7)

#endif  // DEFS_H
