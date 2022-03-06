// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#include "defs.h"

#include <string.h>

extern message_t currentUpdate;

/**
 * Extracts and returns the PGN from a CAN message structure
 * @param message_t *message The message from which to get the PGN
 * @return uint32_t
 */
inline uint32_t cmdPGNFromMessage(message_t *message)
{
    return (message->id & 0x3ffff00) >> 8;  // J1939 parameter group number (PGN) should be id[25:8]
}

/**
 * Sends a command to the next upstream device.
 * @return OK if the command was sent successfully, ERR otherwise
 */
uint8_t cmdSendUpstream(message_t *command)
{
    if (CANSend(UP, &command->id, CAN_FRAME_EXT, 0x00, CAN_DATA_LEN_MAX,
                (uint8_t **)&command->data) != OK)
        return ERR;
    return OK;
}

/**
 * Receives a command from the next downstream device.
 * @param message_t *command Pointer to the message structure to populate with received CAN data
 * @return 0x01 on a successful receive, 0x00 otherwise
 */
uint8_t cmdReceiveDownstream(message_t *command)
{
    uint8_t received;

    uint32_t id;
    uint8_t data[CAN_DATA_LEN_MAX];

    if ((received = CANReceive(DOWN, &id, (uint8_t **)&data)) != NOP) {
        command->id = id;
        memcpy(command->data, data, CAN_DATA_LEN_MAX);
    }

    return received;
}

/**
 * Parses a command to determine if it should execute on this device.
 * @param message_t *command The command to be parsed
 * @return OK if there were no problems parsing the command, ERR otherwise
 */
uint8_t cmdParse(message_t *command)
{
    // TODO Implement this properly
    // What to do here:
    //  - read command->id
    //  - determine if the PGN matches one that is useful for this device
    //  - if it is, convert command->data appropriately and call the right function

    uint32_t pgn = cmdPGNFromMessage(command);
    switch (pgn) {
        case PGN_DEBUG_HANDSHAKE: {
            updateSendDownstream(&currentUpdate);  // Just send the message right back
            break;
        }
        case PGN_TARE_START: {

            break;
        }
        case PGN_TARE_STEP1: {

            break;
        }
        case PGN_TARE_STEP2: {

            break;
        }
        case PGN_TARE_FINISH: {

            break;
        }
        case PGN_CALIBRATE_START: {

            break;
        }
        case PGN_CALIBRATE_FINISH: {

            break;
        }
        default: {
            // Unrecognized command
            return ERR;
        }
    }

    return OK;
}

