//
// Created by jeremy on 2022-03-23.
//

#include "defs.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

SoftwareSerial *serUp;
AltSoftSerial *serDown;

class Mailbox {
private:
    bool receiving;
    String content;

public:
    void Push(char c)
    {
        if (c == '{') {
            content = "";
            receiving = true;

        } else if (c == '}') {
            receiving = false;
            Serial.println("Got " + content);  // TODO remove

        } else {
            content += c;
        }
    }

    bool IsReady() const { return !receiving && content.length() > 0; }

    String Read()
    {
        if (receiving) return "";
        return content;
    }
};

struct Parser {
    Parser() = delete;

    static String data;

    static void LoadData(String str) { data = str; }

    static bool IsCommand() { return data.charAt(0) == '!'; }
    static cmd_type_t GetCommandType()
    {
        int idx;
        if ((idx = data.indexOf("TYPE")) < 0) return Invalid;
        if ((idx = data.indexOf(':', idx)) < 0) return Invalid;
        switch (data.charAt(idx+1)) {
            case 't': return Tare;
            case 'c': return Calibrate;
            case 'r': return Reset;
            default:  return Invalid;
        }
    }
};

String Parser::data = "";

Mailbox mboxUp, mboxDown;

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
    // TODO Changed this, make sure it works
    String toSend = uartSerializeMessage(message);
    Stream *uart = dir == Up ? static_cast<Stream *>(serUp) : static_cast<Stream *>(serDown);
    return uart->print(toSend);
}

bool uartReceive(dir_t dir)
{
    // TODO Changed this, make sure it works
    Stream *uart = dir == Up ? static_cast<Stream *>(serUp) : static_cast<Stream *>(serDown);
    Mailbox *mbox = dir == Up ? &mboxUp : &mboxDown;
    while (uart->available() > 0) {
        mbox->Push(uart->read());
    }
    return mbox->IsReady();
}

void uartGetMessage(dir_t dir, message_t *message)
{
    Mailbox *mbox = dir == Up ? &mboxUp : &mboxDown;
    Parser::LoadData(mbox->Read());

    if (Parser::IsCommand()) {
        cmd_type_t type = Parser::GetCommandType();
    }
}
