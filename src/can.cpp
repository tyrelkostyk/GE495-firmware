// can.c
// Contains low-level CAN-specific communication functions, including configuration.

#include "defs.h"

#ifdef _ARDUINO
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
#endif

/**
 * Performs initialization and configuration for the CAN communication system.
 * @return void
 */
void CANSetup(void)
{
#ifdef Arduino_h
    serUp.begin(CAN_BAUDRATE);
    serDown.begin(CAN_BAUDRATE);
    canUp.begin(serUp, CAN_BAUDRATE);
    canDown.begin(serDown, CAN_BAUDRATE);

    debugPrintLine("Setting upstream mask...");
    if (!canUp.setMask(rx_mask)) {
        debugPrintLine("Upstream mask not set");
    }
    debugPrintLine("Setting upstream filter...");
    if (!canUp.setFilt(rx_filter)) {
        debugPrintLine("Upstream filter not set");
    }

    debugPrintLine("Setting downstream mask...");
    if (!canDown.setMask(rx_mask)) {
        debugPrintLine("Downstream mask not set");
    }
    debugPrintLine("Setting downstream filter...");
    if (!canDown.setFilt(rx_filter)) {
        debugPrintLine("Downstream filter not set");
    }

#endif
}

/**
 * Sends a message with specified ID and length to the CAN controller to be transmitted.
 * @param can_dir_t direction Whether to send upstream or downstream
 * @param uint32_t id The identifier for the message (29 bits in J1939)
 * @param uint8_t ext Set to 0x01 for extended frame, 0x00 for standard frame
 * @param uint8_t rtr Set to 0x01 for Remote Transmission Requests (broadcast)
 * @param uint8_t length Length of the data message in bytes
 * @param uint8_t *data Array of data bytes (doesn't have to NULL-terminate)
 * @return void
 */
void CANSend(can_dir_t direction, uint32_t id, uint8_t ext, uint8_t rtr, uint8_t length, const uint8_t *data)
{
#ifdef Arduino_h
    if (direction == UP)
        canUp.send(id, ext, rtr, length, data);
    else if (direction == DOWN)
        canDown.send(id, ext, rtr, length, data);
#endif
}

/**
 * Checks if the CAN controller has data ready to be read and, if so, reads the data 
 * into the specified buffer.
 * @param uint32_t *id Address to store the message identifier (29 bits in J1939)
 * @param uint8_t *buffer Buffer to which the received message data will be written
 * @return 0x01 if there is data to read, 0x00 otherwise
 */
uint8_t CANReceive(can_dir_t direction, uint32_t *id, uint8_t *buffer)
{
#ifdef Arduino_h
    if (direction == UP)
        return canUp.recv(id, buffer);
    else if (direction == DOWN)
        return canDown.recv(id, buffer);
#endif
}
