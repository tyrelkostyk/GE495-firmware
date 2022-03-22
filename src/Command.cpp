//
// Created by jeremy on 2022-03-22.
//

#include "Command.h"
#include "defs.h"


Logger Command::logger = Logger("Command");
Command Command::current = Command();


void Command::ForwardUpstream()
{
    if (canUp == nullptr) {
        logger.PrintLog(Warn, "No upstream CAN device found");
        return;
    }

    canUp->Send(current);
}


bool Command::ReceiveDownstream()
{
    if (canDown == nullptr) {
        logger.PrintLog(Warn, "No downstream CAN device found");
        return false;
    }

    current = Command();
    if (canDown->Receive(current) == 0) {
        return false;
    }

    current.Load();

    return true;
}


void Command::Parse()
{
    // Check target value, if 0 then take action, otherwise do nothing
    if (current.target != 0) {
        return;
    }

    switch (current.type) {
        case Tare:
            break;

        case Calibrate:
            break;

        case Reset:
            break;

        default:
            logger.PrintLog(Error, "Invalid command type");
            return;
    }
}


void Command::Load()
{
    uint32_t pgn = GetPGN();

    target = (pgn >> PGN_CMD_TARGET_IDX) & PGN_CMD_TARGET;
    type = static_cast<cmd_type_t>((pgn >> PGN_CMD_TYPE_IDX) & PGN_CMD_TYPE);
    arg = (pgn >> PGN_CMD_ARG_IDX) & PGN_CMD_ARG;

    if (type == Calibrate && (arg & PGN_CAL_CONF_M1 || arg & PGN_CAL_CONF_M2)) {
        mass = unpackFloatFromData(data);
    } else {
        mass = 0;
    }
}
