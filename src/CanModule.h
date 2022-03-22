//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_CANMODULE_H
#define CAPSTONE_FIRMWARE_CANMODULE_H


#include "defs.h"
#include "Logger.h"

#include <Arduino.h>
#include <AltSoftSerial.h>
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>


class CanModule : private Serial_CAN, private Logger {

    can_dir_t direction;

    static SoftwareSerial serialUp;
    static AltSoftSerial serialDown;

    static String GetNameForLogger(can_dir_t dir)
    {
        return dir == Up ? "CanUp" : "CanDown";
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

    explicit CanModule(can_dir_t dir) : Logger(GetNameForLogger(dir)), direction(dir)
    {
        if (dir == Up) {
            serialUp.begin(CAN_BAUDRATE);
            Serial_CAN::begin(serialUp, CAN_BAUDRATE);  // SoftwareSerial for upstream
        } else {
            serialDown.begin(CAN_BAUDRATE);
            Serial_CAN::begin(serialDown, CAN_BAUDRATE);  // AltSoftSerial for downstream
        }
    }

    void Init();

    void Send();
    void Receive();

};


#endif //CAPSTONE_FIRMWARE_CANMODULE_H
