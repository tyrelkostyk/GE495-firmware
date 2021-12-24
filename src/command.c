// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

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
 * @return void
 */
command_t *cmdReceiveDownstream(void)
{
    
}

/**
 * Parses a command to determine if it should execute on this device.
 * @param command_t *command The command to be parsed
 * @return void
 */
void cmdParse(command_t *command)
{

}

