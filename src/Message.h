//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_MESSAGE_H
#define CAPSTONE_FIRMWARE_MESSAGE_H


#include "defs.h"

#include <stdint.h>


struct Message {
public:
    uint32_t id;
    uint8_t length;
    uint8_t data[CAN_DATA_LEN_MAX] = { 0 };
};


#endif //CAPSTONE_FIRMWARE_MESSAGE_H
