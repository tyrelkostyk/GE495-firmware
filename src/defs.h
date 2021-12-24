// defs.h
// Contains type definitions and function stubs.

#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

/******
* CAN *
******/

#define CAN_BAUDRATE 9600
#define CAN_RX 4
#define CAN_TX 5

void CANSetup (void);
void CANSend (uint32_t id, uint8_t ext, uint8_t rtr, uint8_t length, const uint8_t *data);
uint8_t CANReceive (uint32_t *id, uint8_t *buffer);

/*********
* SERIAL *
*********/

#define SER_BAUDRATE 115200

/****************
* COMMAND (CMD) *
****************/

typedef struct _command_t {
    uint32_t id;  // TODO: What is the actual size required for this?

    // TODO: What else goes in this struct?
} command_t;

void cmdSendUpstream (void);
void cmdReceiveDownstream (void);
void cmdParse (command_t* command);

#endif  // DEFS_H
