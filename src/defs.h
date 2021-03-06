// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

#define OK  0x01
#define ERR 0x00
#define NOP 0xff

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

#define CAN_BAUDRATE 9600
#define CAN_UP_TX 4
#define CAN_UP_RX 5

// Downstream CAN for Arduino now uses AltSoftSerial which requires RX 8, TX 9
// #define CAN_DOWN_TX 6
// #define CAN_DOWN_RX 7

#define CAN_FRAME_STD 0x00
#define CAN_FRAME_EXT 0x01

#define CAN_DATA_LEN_MAX 8

// Enum for determining which CAN unit is desired (upstream or downstream)
typedef enum {
    UP,
    DOWN
} can_dir_t;

typedef struct _message_t {
    uint32_t id;
    // uint8_t length;  // Unnecessary
    uint8_t data[CAN_DATA_LEN_MAX];
} message_t;

uint8_t CANSetup (void);
uint8_t CANSend (can_dir_t direction, uint32_t id, uint8_t ext, uint8_t rtr, 
                 uint8_t length, const uint8_t *data);
uint8_t CANReceive (can_dir_t direction, uint32_t *id, uint8_t *buffer);

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

// TODO These will definitely have to change

#define PGN_DEBUG_HANDSHAKE 0x01

#define PGN_TARE_START  0x02
#define PGN_TARE_STEP1  0x03
#define PGN_TARE_STEP2  0x04
#define PGN_TARE_FINISH 0x05

#define PGN_CALIBRATE_START  0x06
#define PGN_CALIBRATE_FINISH 0x07

/*********
* UPDATE *
*********/

// TODO Deprecated?
#define UPDATE_MASS_LEN_MAX 8
#define UPDATE_DELAY_MS 1000

uint8_t updateReceiveUpstream(message_t *update);
uint8_t updateSendDownstream(message_t *update);
uint8_t updateHandle(message_t *update);
uint8_t updateLoadCurrentData(message_t *update);

/*******
* MASS *
*******/

#define MASS_NUM_BYTES 4
#define ARDUINO_MASS_PIN A4
#define SAMPLE_DELAY_MS 250

void massGetCurrent(uint8_t *buffer);
void massRead(void);

/*******
* TANK *
*******/

// void tankRefreshID(uint8_t refID);  // Deprecated (for now?)

#endif  // DEFS_H
