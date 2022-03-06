// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#include "defs.h"

#include <Arduino.h>

#include <string.h>


/**
 * Receives an update from the next upstream device.
 * @param message_t *update The message structure to populate with the received CAN data
 * @return 0x01 on a successful receive, 0x00 otherwise
 */
uint8_t updateReceiveUpstream(message_t *update)
{
    uint8_t received;

    uint32_t id;
    uint8_t data[CAN_DATA_LEN_MAX];

    if ((received = CANReceive(UP, &id, (uint8_t **)&data)) != 0x00) {
        update->id = id;
        memcpy(update->data, data, CAN_DATA_LEN_MAX);
    }

    return received;
}

/**
 * Sends an update to the next downstream device.
 * @param message_t *update The message structure to transmit downstream
 * @return OK on successful transmission, ERR otherwise
 */
uint8_t updateSendDownstream(message_t *update)
{
    if (CANSend(DOWN, &update->id, CAN_FRAME_EXT, 0x00, CAN_DATA_LEN_MAX,
                (uint8_t **)&update->data) != OK) {
        return ERR;
    }
    return OK;
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param message_t *update The update to be modified
 * @return OK on a successful handling, ERR if an error occurs
 */
uint8_t updateHandle(message_t *update)
{
    return OK;
}
