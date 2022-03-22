//
// Created by jeremy on 2022-03-22.
//

#ifndef CAPSTONE_FIRMWARE_UPDATE_H
#define CAPSTONE_FIRMWARE_UPDATE_H


#include "CanModule.h"
#include "Logger.h"
#include "Message.h"


class Update : private Message {

    static Logger logger;
    static Update current;

    uint8_t tank;
    float mass;

public:
    Update() = default;

    static void ForwardDownstream();
    static bool ReceiveUpstream();

    static String ToString();

    void IncrementTank();
    static void Handle();
    static void PackData(uint32_t mass);

    void Load() override;

};


#endif //CAPSTONE_FIRMWARE_UPDATE_H
