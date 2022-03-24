//
// Created by jeremy on 2022-03-23.
//

#ifndef CAPSTONE_FIRMWARE_UART_H
#define CAPSTONE_FIRMWARE_UART_H


#include "defs.h"

#include <stdint.h>


struct Message {
    float data;
};

struct Command : public Message {
    uint8_t ttl;
    cmd_type_t type;
    uint8_t step;
};

struct Update : public Message {
    uint8_t tank;
};

#endif //CAPSTONE_FIRMWARE_UART_H
