// can.c
// Contains low-level CAN-specific communication functions, including configuration.

#ifdef _ARDUINO
#include "Serial_CAN_Module.h"

Serial_CAN can;
#endif

#include "defs.h"

/**
 * Performs initialization and configuration for the CAN communication system.
 * @return void
 */
void CANSetup(void)
{
#ifdef Arduino_h
    can.begin(CAN_TX, CAN_RX, CAN_BAUDRATE);
#endif
}

/**
 * Sends a message with specified ID and length to the CAN controller to be transmitted.
 * @param uint32_t id The identifier for the message (29 bits in J1939)
 * @param uint8_t ext Set to 0x01 for extended frame, 0x00 for standard frame
 * @param uint8_t rtr Set to 0x01 for Remote Transmission Requests (broadcast)
 * @param uint8_t length Length of the data message in bytes
 * @param uint8_t *data Array of data bytes (doesn't have to NULL-terminate)
 * @return void
 */
void CANSend(uint32_t id, uint8_t ext, uint8_t rtr, uint8_t length, const uint8_t *data)
{
#ifdef Arduino_h
    can.send(id, ext, rtr, length, data);
#endif
}

/**
 * Checks if the CAN controller has data ready to be read and, if so, reads the data 
 * into the specified buffer.
 * @param uint32_t *id Address to store the message identifier (29 bits in J1939)
 * @param uint8_t *buffer Buffer to which the received message data will be written
 * @return 0x01 if there is data to read, 0x00 otherwise
 */
uint8_t CANReceive(uint32_t *id, uint8_t *buffer)
{
    return can.recv(id, buffer);
}
