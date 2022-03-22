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
    return can->send(message.id, CAN_FRAME_EXT, CAN_NO_REMOTE, message.length, message.data);
}

/**
 * Checks if the CAN controller has data ready to be read and, if so, reads the data 
 * into the specified buffer.
 * @param id Address to store the message identifier (29 bits in J1939)
 * @param buffer Buffer to which the received message data will be written
 * @return The number of bytes received, or 0
 */
uint8_t canReceive(can_dir_t direction, message_t *message)
{
    Serial_CAN *can = direction == Up ? &canUp : &canDown;
    return can->recv((unsigned long *)&message->id, message->data);
}

