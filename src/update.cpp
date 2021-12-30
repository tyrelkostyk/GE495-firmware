// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#ifdef _ARDUINO
#endif  // _ARDUINO

#include <string.h>

#include "defs.h"

/**
 * Receives an update from the next upstream device.
 * @param message_t *update Pointer to the message structure to populate with the received CAN data
 * @return 0x01 on a successful receive, 0x00 otherwise
 */
uint8_t updateReceiveUpstream(message_t *update)
{
    uint8_t received;

    uint32_t id;
    uint8_t data[CAN_DATA_LEN_MAX];

    if (received = CANReceive(UP, &id, data)) {
        update->id = id;
        memcpy(update->data, data, CAN_DATA_LEN_MAX);
    }

    return received;
}

/**
 * Sends an update to the next downstream device.
 * @return void
 */
void updateSendDownstream(message_t *update)
{
    CANSend(DOWN, update->id, CAN_FRAME_EXT, 0x00, update->length, update->data);
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param message_t *update The update to be modified
 * @return void
 */
void updateHandle(message_t *update)
{

}

