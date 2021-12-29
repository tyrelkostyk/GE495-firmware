// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#ifdef _ARDUINO
#include <string.h>
#endif  // _ARDUINO

#include "defs.h"

/**
 * Sends a command to the next upstream device.
 * @return void
 */
void cmdSendUpstream(command_t *command)
{
    CANSend(UP, command->id, CAN_FRAME_EXT, 0x00, command->length, command->data);
}

/**
 * Receives a command from the next downstream device.
 * @param command_t *command Pointer to the command structure to populate with received CAN data
 * @return 0x01 on a successful receive, 0x00 otherwise
 */
uint8_t cmdReceiveDownstream(command_t *command)
{
    uint8_t received;

    uint32_t id;
    uint8_t data[CAN_DATA_LEN_MAX];

    if (received = CANReceive(DOWN, &id, data)) {
        command->id = id;
        command->length = CAN_DATA_LEN_MAX;  // TODO Is there a way to determine the actual length?
        memcpy(command->data, data, CAN_DATA_LEN_MAX);
    }

    return received;
}

/**
 * Parses a command to determine if it should execute on this device.
 * @param command_t *command The command to be parsed
 * @return void
 */
void cmdParse(command_t *command)
{

}

