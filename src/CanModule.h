//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_CANMODULE_H
#define CAPSTONE_FIRMWARE_CANMODULE_H


#include "defs.h"
#include "Logger.h"
#include "Message.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>


class CanModule : private Serial_CAN, private Logger {

    can_dir_t direction;

    SoftwareSerial *serialUp;
    AltSoftSerial *serialDown;

    static String GetNameForLogger(can_dir_t dir)
    {
        return dir == Up ? "CanUp" : "CanDown";
    }
    static SoftwareSerial *NewSoftwareSerial(can_dir_t dir)
    {
        if (dir == Up) return new SoftwareSerial(CAN_UP_RX, CAN_UP_TX);
        return nullptr;
    }
    static AltSoftSerial *NewAltSoftSerial(can_dir_t dir)
    {
        if (dir == Down) return new AltSoftSerial;
        return nullptr;
    }

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

public:

    explicit CanModule(can_dir_t dir) :
            Logger(GetNameForLogger(dir)), direction(dir), serialUp(NewSoftwareSerial(dir)),
            serialDown(NewAltSoftSerial(dir)) { }

    void Init();
    void Send(Message &message);
    uint8_t Receive(Message &message);

};


#endif //CAPSTONE_FIRMWARE_CANMODULE_H
