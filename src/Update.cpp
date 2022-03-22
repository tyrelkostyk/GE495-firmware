//
// Created by jeremy on 2022-03-22.
//

#include "Update.h"


Logger Update::logger = Logger("Update");
Update Update::current = Update();


void Update::ForwardDownstream()
{
    if (canDown == nullptr) {
        logger.PrintLog(Warn, "No downstream CAN device found");
        return;
    }

    canDown->Send(current);
    logger.PrintLog(Info, "Sent update downstream");
}


bool Update::ReceiveUpstream()
{
    if (canUp == nullptr) {
        logger.PrintLog(Warn, "No upstream CAN device found");
        return false;
    }

    current = Update();
    if (canUp->Receive(current) == 0) {
        return false;
    }

    current.Load();

    return true;
}


String Update::ToString()
{
    String output = "Update: ";
    output += String(current.tank) + " - " + String(current.mass, 4);
    return output;
}


void Update::Handle()
{
    // Check target value, if 0 then take action, otherwise do nothing
    if (current.tank != 0) {
        current.IncrementTank();
        return;  // TODO increment nicely
    }
}


void Update::PackData(uint32_t mass)
{
    current.tank = 0;  // No matter what
    current.mass = static_cast<float>(mass);
}


void Update::IncrementTank()
{
    ++tank;

    uint32_t pgn = GetPGN();
    uint32_t tankCheck = (pgn >> PGN_UPDATE_TANK_IDX) & PGN_UPDATE_TANK;
    if (tankCheck < PGN_UPDATE_TANK) {  // Check that we won't overflow the tank's allocated bits
        id += (1 << (PGN_POSITION + PGN_UPDATE_TANK_IDX));  // Add 1 starting at the position of the tank index
    } else {
        logger.PrintLog(Error, "Cannot increment the tank index any further, maximum already reached");
    }
}


void Update::Load()
{
    uint32_t pgn = GetPGN();

    tank = (pgn >> PGN_UPDATE_TANK_IDX) & PGN_UPDATE_TANK;
    mass = unpackFloatFromData(data);
}
