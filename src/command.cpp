// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#ifdef _ARDUINO
#endif  // _ARDUINO

#include "defs.h"

static command_t gCommand = {0};

/**
 * Extracts and returns the PGN from a CAN message structure
 * @param message The message from which to get the PGN
 * @return Unsigned 32-bit integer representing the PGN of the message
 */
inline uint32_t cmdPGNFromMessage(message_t *message)
{
    return (message->id & 0x3ffff00) >> 8;  // J1939 parameter group number (PGN) should be id[25:8]
}

/**
 * Sends a command to the next upstream device.
 * @return OK if the command was sent successfully, ERR otherwise
 */
uint8_t cmdSendUpstream(void)
{
    // TODO convert "gCommand" to a message_t format
    message_t message = { 0 };
    if (canSend(Up, message.id, CAN_FRAME_EXT, 0x00, CAN_DATA_LEN_MAX,
                message.data) != OK)
        return ERR;
    return OK;
}

/**
 * Receives a command from the next downstream device.
 * @param command Pointer to the message structure to populate with received CAN data
 * @return The number of bytes received, or 0
 */
uint8_t cmdReceiveDownstream(void)
{
    // TODO convert message_t to command_t
    return canReceive(Down, &command->id, command->data);
}

/**
 * Parses a command to determine if it should execute on this device.
 * @param command The command to be parsed
 * @return OK if there were no problems parsing the command, ERR otherwise
 */
uint8_t cmdParse(void)
{
    uint32_t pgn = cmdPGNFromMessage(command);

    switch (pgn) {
        case PGN_DEBUG_HANDSHAKE: {
            // updateLoadCurrentData(&currentUpdate);
            // updateSendDownstream(&currentUpdate);  // Just send the message right back
            break;
        }

        case PGN_TARE: {

            break;
        }

        case PGN_CALIBRATE | PGN_CAL_CONF_M1: {

            break;
        }

        case PGN_CALIBRATE | PGN_CAL_CONF_M2: {

            break;
        }

        case PGN_CALIBRATE | PGN_CAL_FINISH: {

            break;
        }

        default: {
            // Unrecognized command
            return ERR;
        }
    }

    debugPrintLine("Parsed a command!");
    return OK;
}

