//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_COMMAND_H
#define CAPSTONE_FIRMWARE_COMMAND_H


#include "CanModule.h"
#include "Logger.h"
#include "Message.h"


class Command : private Message {

    static CanModule *canUp;
    static CanModule *canDown;
    static Logger logger;
    static Command current;

    uint8_t target;
    cmd_type_t type;
    uint8_t arg;
    float mass;

public:
    Command() = default;

    static void Init(CanModule *up, CanModule *down);

    static void ForwardUpstream();
    static bool ReceiveDownstream();

    static void Parse();

    void Load() override;

};


#endif //CAPSTONE_FIRMWARE_COMMAND_H
