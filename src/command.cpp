// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#ifdef _ARDUINO
#endif  // _ARDUINO

#include "defs.h"

#include <string.h>

static command_t gCommand = {0};

inline float unpackFloatFromData(uint8_t *data)
{
    uint32_t num = 0;
    for (int i = 0; i < 4; i++) {
        num |= data[i] << (8*i);
    }
    return static_cast<float>(num);
}

inline void packDataWithFloat(uint8_t *data, float f)
{
    uint32_t num = static_cast<uint32_t>(f);
    for (int i = 0; i < 4; i++) {
        data[i] = (num >> (8*i)) & 0xff;
    }
}

inline void packDataWithZero(uint8_t *data)
{
    memset(data, 0, CAN_DATA_LEN_MAX);
}

/**
 * Populates the static command struct with data from the provided message struct PGN.
 * @param message The message struct to retrieve data from
 */
void cmdGetFromMessage(message_t *message)
{
    uint32_t pgn = (message->id >> PGN_POSITION) & PGN_SIZE;

    gCommand.target = (pgn >> PGN_CMD_TARGET_IDX) & PGN_CMD_TARGET;
    gCommand.type = (pgn >> PGN_CMD_TYPE_IDX) & PGN_CMD_TYPE;
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
        packDataWithFloat(message->data, gCommand.data.mass);
    } else {
        message.length = 0;
        packDataWithZero(message->data);
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
 * @return The number of bytes received, or 0
 */
uint8_t cmdReceiveDownstream(void)
{
    // TODO convert message_t to command_t
    return canReceive(Down, &command->id, command->data);
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

