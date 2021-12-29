// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

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

typedef struct _command_t {
    uint32_t id;
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX];
} command_t;

void cmdSendUpstream (command_t *command);
uint8_t cmdReceiveDownstream (command_t *command);
void cmdParse (command_t *command);

#endif  // DEFS_H
