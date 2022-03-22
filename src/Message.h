//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_MESSAGE_H
#define CAPSTONE_FIRMWARE_MESSAGE_H


#include "defs.h"
#include "Logger.h"

#include <stdint.h>


class CanModule;


struct Message {
protected:
    static CanModule *canUp;
    static CanModule *canDown;
    static Logger logger;

public:
    uint32_t id;
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX] = { 0 };

    static void Init(CanModule *up, CanModule *down);

    uint32_t GetPGN() const
    {
        return (id >> PGN_POSITION) & PGN_SIZE;
    }

    virtual void Load() = 0;
};


#endif //CAPSTONE_FIRMWARE_MESSAGE_H
