// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#ifdef _ARDUINO
#include <Arduino.h>
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

    if ((received = CANReceive(UP, &id, data)) != 0x00) {
#ifdef _DBG
        debugPrintLine("Update received!");
#endif  // _DBG

        update->id = id;
        memcpy(update->data, data, CAN_DATA_LEN_MAX);
    }

    return received;
}

/**
 * Sends an update to the next downstream device.
 * @param message_t *update The message structure to transmit downstream
 * @return void
 */
void updateSendDownstream(message_t *update)
{
#ifdef _DBG
    debugPrintLine("Update sent!");
#endif  // _DBG

    CANSend(DOWN, update->id, CAN_FRAME_EXT, 0x00, update->length, update->data);
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param message_t *update The update to be modified
 * @return void
 */
void updateHandle(message_t *update)
{
    // TODO here:
    //  - inject mass data for this node
    //  - increment origin field by 1
    //  - ???
#ifdef _DBG
    debugPrintLine("Update handled!");
#endif  // _DBG
}

