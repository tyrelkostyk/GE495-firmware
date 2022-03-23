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

    memset(&gUpdate, 0, sizeof(gUpdate));

    gUpdate.tank = (pgn >> PGN_UPDATE_TANK_IDX) & PGN_UPDATE_TANK;
    gUpdate.mass = unpackFloatFromData(message->data);
    Serial.print("Tank received: ");
    Serial.print(gUpdate.tank, HEX);
    Serial.print(" ");
    Serial.print("Mass received: ");
    Serial.print(gUpdate.mass);
    Serial.println();
}

message_t updateConvertToMessage(void)
{
    message_t message;
    memset(&message, 0, sizeof(message));

    uint32_t pgn = 0;
    pgn |= (gUpdate.tank & PGN_UPDATE_TANK) << PGN_UPDATE_TANK_IDX;

    message.id = (pgn & PGN_SIZE) << PGN_POSITION;
    message.length = CAN_DATA_LEN_MAX;  // TODO?

    packDataWithFloat(message.data, gUpdate.mass);

    Serial.print("Update to pack: ");
    Serial.print(gUpdate.tank, HEX);
    Serial.print(" ");
    Serial.print(gUpdate.mass);
    Serial.println();
    Serial.print("Packed: ");
    Serial.print(message.id, HEX);
    Serial.print(" ");
    for (int i = 0; i < CAN_DATA_LEN_MAX; i++) {
        Serial.print(message.data[i], HEX);
        Serial.print(".");
    }
    Serial.println();

    return message;
}

/**
 * Receives an update from the next upstream device.
 * @param update The message structure to populate with the received CAN data
 * @return OK if bytes were received, NOP otherwise
 */
uint8_t updateReceiveUpstream(void)
{
    message_t message;
    memset(&message, 0, sizeof(message));
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
 * @param data The float data to be inserted
 */
void updateLoadCurrentData(double data)
{
    gUpdate.tank = 1;  // Always TODO
    gUpdate.mass = data;
}

