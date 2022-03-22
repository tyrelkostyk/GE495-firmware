// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#ifdef _ARDUINO
#endif  // _ARDUINO

#include "defs.h"

static command_t gCommand = {0};

/**
 * Populates the static command struct with data from the provided message struct PGN.
 * @param message The message struct to retrieve data from
 */
void cmdGetFromMessage(message_t *message)
{
    uint32_t pgn = (message->id >> PGN_POSITION) & PGN_SIZE;

    gCommand.target = (pgn >> PGN_CMD_TARGET_IDX) & PGN_CMD_TARGET;
    gCommand.type = static_cast<cmd_type_t>((pgn >> PGN_CMD_TYPE_IDX) & PGN_CMD_TYPE);
    gCommand.arg = (pgn >> PGN_CMD_ARG_IDX) & PGN_CMD_ARG;

    if (gCommand.type == Calibrate) {
        gCommand.data.mass = unpackFloatFromData(message->data);
    }
}

message_t cmdConvertToMessage(void)
{
    message_t message = { 0 };
    message.id |= (gCommand.target & PGN_CMD_TARGET) << PGN_CMD_TARGET_IDX;
    message.id |= (gCommand.type & PGN_CMD_TYPE) << PGN_CMD_TYPE_IDX;
    message.id |= (gCommand.arg & PGN_CMD_ARG) << PGN_CMD_ARG_IDX;

    if (gCommand.type == Calibrate) {
        message.length = CAN_DATA_LEN_CALIBRATE;
        packDataWithFloat(message.data, gCommand.data.mass);
    } else {
        message.length = 0;
        packDataWithZero(message.data);
    }

    return message;
}

/**
 * Sends a command to the next upstream device.
 * @return OK if the command was sent successfully, ERR otherwise
 */
uint8_t cmdSendUpstream(void)
{
    message_t message = cmdConvertToMessage();
    if (canSend(Up, message.id, CAN_FRAME_EXT, 0x00, message.length,
                message.data) != OK)
        return ERR;
    return OK;
}

/**
 * Receives a command from the next downstream device.
 * @param command Pointer to the message structure to populate with received CAN data
 * @return OK if bytes were received, NOP otherwise
 */
uint8_t cmdReceiveDownstream(void)
{
    message_t message = { 0 };
    int received = canReceive(Down, &message.id, message.data);
    if (received > 0) {
        cmdGetFromMessage(&message);
    }
    return received > 0 ? OK : NOP;
}

/**
 * Parses a command to determine if it should execute on this device.
 * @return OK if there were no problems parsing the command, NOP if this device is not the target, ERR otherwise
 */
uint8_t cmdParse(void)
{
    // Check target value, if 0 then take action, otherwise continue
    // Decrement after check
    if (gCommand.target-- != 0) {
        return NOP;
    }

    switch (gCommand.type) {
        case Tare:
            break;

        case Calibrate:
            break;

        case Reset:
            break;

        default:
            return ERR;
    }

    debugPrintLine("Parsed a command!");
    return OK;
}

