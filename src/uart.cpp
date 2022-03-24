//
// Created by jeremy on 2022-03-23.
//

#include "defs.h"
#include "uart.h"

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
    bool Push(char c)
    {
        if (c == '{') {
            content = "";
            receiving = true;

        } else if (c == '}') {
            receiving = false;
            return true;

        } else {
            if (receiving)
                content += c;
        }
        return false;
    }

    bool Ready() { receiving = false; }

    bool IsReady() const { return (!receiving) && content.length() > 0; }

    String Read()
    {
        if (receiving) return "";
        String copy(content);
        content = "";
        return copy;
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

    static uint8_t GetCalibrationStep()
    {
        int idx;
        if ((idx = data.indexOf("STEP")) < 0) return -1;
        if ((idx = data.indexOf(':', idx)) < 0) return -1;
        String step = data.substring(idx+1, idx+2);
        return (uint8_t)step.toInt();
    }

    static float GetData()
    {
        int idx;
        if ((idx = data.indexOf("DATA")) < 0) return -1;
        if ((idx = data.indexOf(':', idx)) < 0) return -1;
        String sub = data.substring(idx+1);
        return sub.toFloat();
    }

    static uint8_t GetTTL()
    {
        int idx;
        if ((idx = data.indexOf("TTL")) < 0) return 0;
        if ((idx = data.indexOf(':', idx)) < 0) return 0;
        return (uint8_t)data.substring(idx+1).toInt();
    }

    static uint8_t GetTank()
    {
        int idx;
        if ((idx = data.indexOf("TANK")) < 0) return 0;
        if ((idx = data.indexOf(':', idx)) < 0) return 0;
        return (uint8_t)data.substring(idx+1).toInt();
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

String uartSerializeUpdate(Update *update)
{
    String ser = "{";
    ser += "TANK:" + String(update->tank) + ";";
    ser += "DATA:" + String(update->data, 6) + ";";
    ser += "}";
    return ser;
}

String uartSerializeCommand(Command *command)
{
    String ser = "{!";
    // ttl type step data
    ser += "TTL:" + String(command->ttl) + ";";
    char t;
    switch (command->type) {
        case Tare: t = 't'; break;
        case Calibrate: t = 'c'; break;
        case Reset: t = 'r'; break;
        default: t = 'x';
    }
    ser += "TYPE:" + String(t) + ";";
    if (command->type == Calibrate) ser += "STEP:" + String(command->step) + ";";
    ser += "DATA:" + String(command->data, 6) + ";";
    ser += "}";
    return ser;
}

uint8_t uartSend(dir_t dir, Message *message)
{
    String toSend = dir == Up ?
            uartSerializeCommand((Command *)message)
            : uartSerializeUpdate((Update *)message);
    Stream *uart = dir == Up ? static_cast<Stream *>(serUp) : static_cast<Stream *>(serDown);
    return uart->print(toSend);
}

bool uartReceive(dir_t dir)
{
    Stream *uart = dir == Up ? static_cast<Stream *>(serUp) : static_cast<Stream *>(serDown);
    Mailbox *mbox = dir == Up ? &mboxUp : &mboxDown;
    bool ready = false;
    while (uart->available() > 0) {
        if (mbox->Push(uart->read())) {
            ready = true;
            break;
        }
    }
    if (ready) {
        mbox->Ready();
    }
    return ready;
}

void uartGetMessage(dir_t dir, Message *message)
{
    Mailbox *mbox = dir == Up ? &mboxUp : &mboxDown;
    Parser::LoadData(mbox->Read());

    if (Parser::IsCommand()) {
        Command *command = (Command *)message;
        command->type = Parser::GetCommandType();
        if (command->type == Calibrate) {
            command->step = Parser::GetCalibrationStep();
        }
        command->data = Parser::GetData();
        command->ttl = Parser::GetTTL();
    } else {
        Update *update = (Update *)message;
        update->tank = Parser::GetTank();
        update->data = Parser::GetData();
    }
}
