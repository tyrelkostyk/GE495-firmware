// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#include "defs.h"

#include <Arduino.h>

#include <string.h>

static update_t gUpdate;

/**
 * Populates the static command struct with data from the provided message struct PGN.
 * @param message The message struct to retrieve data from
 */
void updateGetFromMessage(message_t *message)
{
    uint32_t pgn = (message->id >> PGN_POSITION) & PGN_SIZE;

    gUpdate.tank = (pgn >> PGN_UPDATE_TANK_IDX) & PGN_UPDATE_TANK;
    gUpdate.mass = unpackFloatFromData(message->data);
}

message_t updateConvertToMessage(void)
{
    message_t message = { 0 };

    uint32_t pgn = 0;
    pgn |= (gUpdate.tank & PGN_UPDATE_TANK) << PGN_UPDATE_TANK_IDX;

    message.id = (pgn & PGN_SIZE) << PGN_POSITION;

    packDataWithFloat(message.data, gUpdate.mass);

    return message;
}

/**
 * Receives an update from the next upstream device.
 * @param update The message structure to populate with the received CAN data
 * @return OK if bytes were received, NOP otherwise
 */
uint8_t updateReceiveUpstream(void)
{
    // TODO convert message_t to update_t
    message_t message = { 0 };
    uint8_t received = canReceive(Up, &message);
    if (received > 0) {
        updateGetFromMessage(&message);
        return OK;
    }
    return NOP;
}

/**
 * Sends an update to the next downstream device.
 * @param update The message structure to transmit downstream
 * @return OK on successful transmission, ERR otherwise
 */
uint8_t updateSendDownstream(void)
{
    // TODO convert update_t to message_t
    message_t message = updateConvertToMessage();
    if (canSend(Down, message) != OK)
        return ERR;
    return OK;
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param update The update to be modified
 * @return OK on successful handling, ERR if an error occurs
 */
uint8_t updateHandle(void)
{
    Serial.print("ID now ");
    Serial.println(++gUpdate.tank, HEX);

    return OK;
}

/**
 * Inserts current data from mass module into a new update message.
 * @param update The message structure to be reset and modified
 * @return OK or ERR
 */
void updateLoadCurrentData(void)
{
    gUpdate.tank = 0;  // Always
    gUpdate.mass = 0.0;  // TODO get data from the ADC
}

