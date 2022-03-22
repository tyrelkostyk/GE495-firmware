//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_COMMAND_H
#define CAPSTONE_FIRMWARE_COMMAND_H


#include "Message.h"


class Command : private Message {

public:
    Command() { }

    static void SendUpstream(Command *command);
    static Command *ReceiveFromDownstream();

    void Parse();

};


#endif //CAPSTONE_FIRMWARE_COMMAND_H
