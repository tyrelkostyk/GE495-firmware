//
// Created by jeremy on 2022-03-22.
//

#include "CanModule.h"


SoftwareSerial CanModule::serialUp = SoftwareSerial(CAN_UP_RX, CAN_UP_TX);

void CanModule::Init()
{
    if (!setMask(rx_mask)) {
        PrintLog(Warn, "Unable to set mask");
        return;
    }

    if (!setFilt(rx_filter)) {
        PrintLog(Warn, "Unable to set filter");
        return;
    }
}


void CanModule::Send()
{

}


void CanModule::Receive()
{

}
