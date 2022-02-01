// update.c
// Contains functions for receiving and forwarding messages received from upstream sources.

#ifdef _ARDUINO
#include <Arduino.h>
#endif  // _ARDUINO

#include <string.h>

#include "defs.h"

/**
 * Retrieves a tank ID from the data field of a message instance.
 * @param message_t *message The message structure from which to extract the tank ID
 * @return uint8_t The tank ID number from the message
 */
inline void updateMessageTankID(message_t *message)
{
    // Little-endian data transmission means ID will be at the end
    ++(message->data[CAN_DATA_LEN_MAX-1]);
}

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

    if ((received = CANReceive(UP, &id, data)) != 0x00) {
        debugPrintLine("Received update!");
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
    if (CANSend(DOWN, update->id, CAN_FRAME_EXT, 0x00, CAN_DATA_LEN_MAX, 
                update->data) != OK) {
        return ERR;
    }
    debugPrintLine("Sent update!");
    return OK;
}

/**
 * Handles the modification of updates with tagging and mass data.
 * @param message_t *update The update to be modified
 * @return OK on a successful handling, ERR if an error occurs
 */
uint8_t updateHandle(message_t *update)
{
    updateMessageTankID(update);

#ifdef _DBG
    char id[8];
    char mass[CAN_DATA_LEN_MAX-1][8];

    itoa(update->data[CAN_DATA_LEN_MAX-1], id, 16);
    debugPrint("Update handled -- (ID now ");
    debugPrint(id);
    debugPrint(", Mass: ");
    for (int i = 0; i < CAN_DATA_LEN_MAX-1; i++) {
        itoa(update->data[i], mass[i], 16);
        debugPrint(mass[i]);
        debugPrint(" ");
    }
    debugPrintLine(")");
#endif  // _DBG

    return OK;
}

/**
 * Inserts current data from mass module into a new update message.
 * @param message_t *update The message structure to be reset and modified
 * @return OK or ERR
 */
uint8_t updateLoadCurrentData(message_t *update)
{
    uint8_t mass[MASS_NUM_BYTES];

    update->id = 0x0;  // TODO Set the PGN correctly
    if (memset(update->data, 0, CAN_DATA_LEN_MAX) == NULL)
        return ERR;

    massGetCurrent(mass);
    if (memcpy((uint8_t *)(&update->data), (uint8_t *)mass, MASS_NUM_BYTES) == NULL)
        return ERR;

    update->data[CAN_DATA_LEN_MAX-1] = 0x00;

    return OK;
}

