//
// Created by jeremy on 2022-03-22.
//

#include "CanModule.h"


void CanModule::Init()
{
    if (direction == Up) {
        if (serialUp == nullptr) {
            PrintLog(Error, "Upstream CAN has no software serial instance");
            return;
        }
        serialUp->begin(CAN_BAUDRATE);
        Serial_CAN::begin(*serialUp, CAN_BAUDRATE);  // SoftwareSerial for upstream
    } else {
        if (serialDown == nullptr) {
            PrintLog(Error, "Downstream CAN has no alt soft serial instance");
            return;
        }
        serialDown->begin(CAN_BAUDRATE);
        Serial_CAN::begin(*serialDown, CAN_BAUDRATE);  // AltSoftSerial for downstream
    }

    if (!setMask(rx_mask)) {
        PrintLog(Warn, "Unable to set mask");
        return;
    }

    if (!setFilt(rx_filter)) {
        PrintLog(Warn, "Unable to set filter");
        return;
    }
}


void CanModule::Send(Message &message)
{
    send(message.id, CAN_FRAME_EXT, CAN_NO_REMOTE, message.length, message.data);
}


uint8_t CanModule::Receive(Message &message)
{
    return recv((unsigned long *)&message.id, message.data);
}
