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

    debugPrintLine("Setting upstream mask...");
    if (!canUp.setMask(rx_mask)) {
        debugPrintLine("Upstream mask not set, skipping upstream filter...");
        setupStatus = ERR;
    } else {
        debugPrintLine("Setting upstream filter...");
        if (!canUp.setFilt(rx_filter)) {
            debugPrintLine("Upstream filter not set");
            setupStatus = ERR;
        }
    }

    debugPrintLine("Setting downstream mask...");
    if (!canDown.setMask(rx_mask)) {
        debugPrintLine("Downstream mask not set, skipping downstream filter...");
        setupStatus = ERR;
    } else {
        debugPrintLine("Setting downstream filter...");
        if (!canDown.setFilt(rx_filter)) {
            debugPrintLine("Downstream filter not set");
            setupStatus = ERR;
        }
    }

    return setupStatus;
}

/**
 * Sends a message with specified ID and length to the CAN controller to be transmitted.
 * @param direction Whether to send upstream or downstream
 * @param id The identifier for the message (29 bits in J1939)
 * @param ext Set to 0x01 for extended frame, 0x00 for standard frame
 * @param rtr Set to 0x01 for Remote Transmission Requests (broadcast)
 * @param length Length of the data message in bytes
 * @param data Array of data bytes (doesn't have to NULL-terminate)
 * @return OK on success, ERR otherwise
 */
uint8_t canSend(can_dir_t direction, uint32_t id, uint8_t ext, uint8_t rtr, uint8_t length, const uint8_t *data)
{
    if (direction == Up)
        return canUp.send(id, ext, rtr, length, data);
    else if (direction == Down)
        return canDown.send(id, ext, rtr, length, data);
}

/**
 * Checks if the CAN controller has data ready to be read and, if so, reads the data 
 * into the specified buffer.
 * @param id Address to store the message identifier (29 bits in J1939)
 * @param buffer Buffer to which the received message data will be written
 * @return The number of bytes received, or 0
 */
uint8_t canReceive(can_dir_t direction, uint32_t *id, uint8_t *buffer)
{
    uint8_t received;
    if (direction == Up) {
        received = canUp.recv(id, buffer);
    }
    else if (direction == Down) {
        received = canDown.recv(id, buffer);
    }
    return received;
}

