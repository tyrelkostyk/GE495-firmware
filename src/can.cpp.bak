// can.c
// Contains low-level CAN-specific communication functions, including configuration.

#include "defs.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include "Serial_CAN_Module.h"

uint32_t rx_mask[4] = {
    0x1, 0x0,
    0x1, 0x0
};

uint32_t rx_filter[12] = {
    0x1, 0xffffffff,
    0x1, 0xffffffff,
    0x1, 0xffffffff,
    0x1, 0xffffffff,
    0x1, 0xffffffff,
    0x1, 0xffffffff
};

Serial_CAN canUp;
Serial_CAN canDown;

SoftwareSerial serUp = SoftwareSerial(CAN_UP_RX, CAN_UP_TX);
AltSoftSerial serDown;

/**
 * Performs initialization and configuration for the CAN communication system.
 * @return OK on successful setup, ERR if anything has not initialized correctly
 */
uint8_t canInit(void)
{
    uint8_t setupStatus = OK;

    serUp.begin(CAN_BAUDRATE);
    serDown.begin(CAN_BAUDRATE);
    canUp.begin(serUp, CAN_BAUDRATE);
    canDown.begin(serDown, CAN_BAUDRATE);

    Serial.println("Setting upstream mask...");
    if (!canUp.setMask(rx_mask)) {
        Serial.println("Upstream mask not set, skipping upstream filter...");
        setupStatus = ERR;
    } else {
        Serial.println("Setting upstream filter...");
        if (!canUp.setFilt(rx_filter)) {
            Serial.println("Upstream filter not set");
            setupStatus = ERR;
        }
    }

    Serial.println("Setting downstream mask...");
    if (!canDown.setMask(rx_mask)) {
        Serial.println("Downstream mask not set, skipping downstream filter...");
        setupStatus = ERR;
    } else {
        Serial.println("Setting downstream filter...");
        if (!canDown.setFilt(rx_filter)) {
            Serial.println("Downstream filter not set");
            setupStatus = ERR;
        }
    }

    return setupStatus;
}

/**
 * Sends a message with specified ID and length to the CAN controller to be transmitted.
 * @param direction Whether to send upstream or downstream
 * @param message The message data to be transmitted
 * @return OK on success, ERR otherwise
 */
uint8_t canSend(can_dir_t direction, message_t message)
{
    Serial_CAN *can = direction == Up ? &canUp : &canDown;
    Serial.print("Sending message: ");
    Serial.print(message.id, HEX);
    Serial.print(" ");
    for (int i = 0; i < CAN_DATA_LEN_MAX; i++) {
        Serial.print(message.data[i], HEX);
        Serial.print(".");
    }
    Serial.println();
    return can->send(message.id, CAN_FRAME_EXT, CAN_NO_REMOTE, CAN_DATA_LEN_MAX, message.data);
}

/**
 * Checks if the CAN controller has data ready to be read and, if so, reads the data 
 * into the specified buffer.
 * @param direction Up or Down
 * @param message Message instance to populate with received data
 * @return The number of bytes received, or 0
 */
uint8_t canReceive(can_dir_t direction, message_t *message)
{
    Serial_CAN *can = direction == Up ? &canUp : &canDown;
    uint32_t id = 0;
    uint8_t data[CAN_DATA_LEN_MAX] = {0};
    uint8_t received = can->recv(&id, data);
    Serial.print("Received: ");
    Serial.print(id, HEX);
    Serial.print(" ");
    for (int i = 0; i < CAN_DATA_LEN_MAX; i++) {
        Serial.print(data[i], HEX);
        Serial.print(".");
    }
    Serial.println();
    // Trying this copying approach to see if it works better than just passing the addresses of the message fields
    message->id = id;
    memcpy(message->data, data, CAN_DATA_LEN_MAX);
    return received;
}

