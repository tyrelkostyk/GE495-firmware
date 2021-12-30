// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

/********
* DEBUG *
********/

#define DEBUG_INPUT_LEN_MAX (1<<7)

void debugPrint(const char *message);
void debugPrintLine(const char *message);
uint32_t debugReadLine(char *buffer);
void debugScan(void);
void debugHandshake(void);

/******
* CAN *
******/

#define CAN_BAUDRATE 9600
#define CAN_UP_RX 4
#define CAN_UP_TX 5
#define CAN_DOWN_RX 6
#define CAN_DOWN_TX 7

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
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX];
} message_t;

void CANSetup (void);
void CANSend (can_dir_t direction, uint32_t id, uint8_t ext, uint8_t rtr, uint8_t length, const uint8_t *data);
uint8_t CANReceive (can_dir_t direction, uint32_t *id, uint8_t *buffer);

/*********
* SERIAL *
*********/

#define SER_BAUDRATE 115200

/****************
* COMMAND (CMD) *
****************/

void cmdSendUpstream (message_t *command);
uint8_t cmdReceiveDownstream (message_t *command);
void cmdParse (message_t *command);

// TODO These will definitely have to change

#define PGN_DEBUG_HANDSHAKE 0x00

#define PGN_TARE_START  0x01
#define PGN_TARE_STEP1  0x02
#define PGN_TARE_STEP2  0x03
#define PGN_TARE_FINISH 0x04

#define PGN_CALIBRATE_START  0x05
#define PGN_CALIBRATE_FINISH 0x06

/*********
* UPDATE *
*********/

// TODO Deprecated?
#define UPDATE_MASS_LEN_MAX 8

uint8_t updateReceiveUpstream(message_t *update);
void updateSendDownstream(message_t *update);
void updateHandle(message_t *update);

#endif  // DEFS_H
