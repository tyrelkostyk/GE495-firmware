// command.c
// Contains functions for parsing and carrying out commands received from downstream sources including ECU.

#include "defs.h"

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <stdlib.h>
#include <string.h>

extern message_t currentUpdate;

/**
 * Extracts and returns the PGN from a CAN message structure
 * @param message_t *message The message from which to get the PGN
 * @return uint32_t
 */
inline uint32_t cmdPGNFromMessage(message_t *message)
{
    return (message->id & 0x3ffff00) >> 8;  // J1939 parameter group number (PGN) should be id[25:8]
}

/**
 * Constructs a CAN ID by building a PGN from the provided code and inserting the ID mask.
 * @param code The command code received from the PC
 * @return uint32_t
 */
inline uint32_t cmdConstructID(uint8_t code)
{
    return (uint32_t)(code << 8);
}

/**
 * Sends a command to the next upstream device.
 * @return OK if the command was sent successfully, ERR otherwise
 */
uint8_t cmdSendUpstream(message_t *command)
{
    if (CANSend(UP, &command->id, CAN_FRAME_EXT, 0x00, CAN_DATA_LEN_MAX,
                (uint8_t **)&command->data) != OK)
        return ERR;
    return OK;
}

/**
 * Receives a command from the next downstream device.
 * @param message_t *command Pointer to the message structure to populate with received CAN data
 * @return 0x01 on a successful receive, 0x00 otherwise
 */
uint8_t cmdReceiveDownstream(message_t *command)
{
    uint8_t received = 1;

    String s = Serial.readStringUntil(';');

    Serial.println("Got " + s);
    // Serial.println(received);

    switch (s[0]) {
        case 'T':
            // construct a tare command
            Serial.println("TARE");
            break;
        case 'R':
            // cycle power on the Arduino
            Serial.println("RESET");
            break;
        case 'Z':
            switch (s[1]) {
                case '0':
                    // construct a start zero command
                    Serial.println("ZERO-START");
                    break;
                case '1':
                    // construct a zero step-1 command
                    Serial.println("ZERO-M1");
                    break;
                case '2':
                    // construct a zero step-2 command
                    Serial.println("ZERO-M2");
                    break;
                default:
                    return 0;
            }
            break;
        default:
            Serial.println("Done...");
            return 0;
    }

    Serial.println("Received");
    return received;
}

/**
 * Parses a command to determine if it should execute on this device.
 * @param message_t *command The command to be parsed
 * @return OK if there were no problems parsing the command, ERR otherwise
 */
uint8_t cmdParse(message_t *command)
{
    // TODO Implement this properly
    // What to do here:
    //  - read command->id
    //  - determine if the PGN matches one that is useful for this device
    //  - if it is, convert command->data appropriately and call the right function

    uint32_t pgn = cmdPGNFromMessage(command);
    switch (pgn) {
        case PGN_DEBUG_HANDSHAKE: {
            Serial.println("Hello, world!");
            break;
        }
        case PGN_TARE: {

            break;
        }
        case PGN_CALIBRATE | PGN_CAL_CONF_M1: {

            break;
        }
        case PGN_CALIBRATE | PGN_CAL_CONF_M2: {

            break;
        }
        case PGN_CALIBRATE | PGN_CAL_FINISH: {

            break;
        }
        default: {
            // Unrecognized command
            return ERR;
        }
    }

    return OK;
}
