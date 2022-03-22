// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#include "defs.h"

#include <Arduino.h>

#include <string.h>

static update_t gUpdate;

/**
 * Retrieves a tank ID from the data field of a message instance.
 * @param message The message structure from which to extract the tank ID
 * @return The tank ID number from the message
 */
inline void updateMessageTankID(message_t *message)
{
    // Little-endian data transmission means ID will be at the end
    ++(message->data[CAN_DATA_LEN_MAX-1]);
}

/**
 * Receives an update from the next upstream device.
 * @param update The message structure to populate with the received CAN data
 * @return The number of bytes received, or 0
 */
uint8_t updateReceiveUpstream(void)
{
    // TODO convert message_t to update_t
    message_t message = { 0 };
    return canReceive(Up, &message);
}

/**
 * Sends an update to the next downstream device.
 * @param update The message structure to transmit downstream
 * @return OK on successful transmission, ERR otherwise
 */
uint8_t updateSendDownstream(void)
{
    // TODO convert update_t to message_t
    message_t message = { 0 };
    if (canSend(Down, message) != OK) {
        return ERR;
    }
    return OK;
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param update The update to be modified
 * @return OK on successful handling, ERR if an error occurs
 */
uint8_t updateHandle(void)
{
    updateMessageTankID(update);

    Serial.print("ID now ");
    Serial.println(update->data[CAN_DATA_LEN_MAX-1], HEX);

    return OK;
}

/**
 * Inserts current data from mass module into a new update message.
 * @param update The message structure to be reset and modified
 * @return OK or ERR
 */
uint8_t updateLoadCurrentData(void)
{
    uint8_t mass[MASS_NUM_BYTES] = { 0 };

    update->id = 0x0;  // TODO Set the PGN correctly
    if (memcpy((uint8_t *)(&update->data), (uint8_t *)mass, MASS_NUM_BYTES) == NULL)
        return ERR;

    update->data[CAN_DATA_LEN_MAX-1] = 0x00;

    return OK;
}

