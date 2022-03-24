//
// Created by jeremy on 2022-03-23.
//

#include "defs.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

SoftwareSerial *serUp;
AltSoftSerial *serDown;

String upMailbox, downMailbox;
bool upLocked, downLocked;
bool upReceived, downReceived;

void uartInit(void)
{
    serUp = new SoftwareSerial(CAN_UP_RX, CAN_UP_TX);
    serUp->begin(UART_BAUDRATE);
    serUp->setTimeout(100);
    serUp->flush();

    serDown = new AltSoftSerial;
    serDown->begin(UART_BAUDRATE);
    serDown->setTimeout(100);
    serDown->flush();
}

String uartSerializeMessage(message_t *message)
{
    return R"({"Hello, world!"})";
}

uint8_t uartSend(dir_t dir, message_t *message)
{
    size_t sent = 0;
    String toSend = uartSerializeMessage(message);
    if (dir == Up) {
        sent = serUp->print(toSend);
    } else {
        sent = serDown->print(toSend);
    }
    return sent;
}

void uartReceive(dir_t dir, message_t *message)
{
    size_t received = 0;
    char c;
    if (dir == Up) {
        while (serUp->available() > 0) {
            c = serUp->read();
            if (c == '{') {
                upMailbox = "";
                upReceived = false;
                upLocked = true;
            } else if (c == '}') {
                upReceived = true;
                upLocked = false;
                Serial.println("Got " + upMailbox);
            } else {
                if (upLocked)
                    upMailbox += c;
            }
        }
    } else {
        while (serDown->available() > 0) {
            c = serDown->read();
            if (c == '{') {
                downMailbox = "";
                downReceived = false;
                downLocked = true;
            } else if (c == '}') {
                downReceived = true;
                downLocked = false;
                Serial.println("Got " + downMailbox);
            } else {
                if (downLocked)
                    downMailbox += c;
            }
        }
    }
}