//
// Created by jeremy on 2022-03-22.
//

#include "Command.h"
#include "defs.h"


CanModule *Command::canUp = nullptr;
CanModule *Command::canDown = nullptr;
Logger Command::logger = Logger("Command");
Command Command::current = Command();


void Command::Init(CanModule *up, CanModule *down)
{
    Command::canUp = up;
    Command::canDown = down;
}


void Command::ForwardUpstream()
{
    if (Command::canUp == nullptr) {
        logger.PrintLog(Warn, "No upstream CAN device found");
        return;
    }

    canUp->Send(Command::current);
}


bool Command::ReceiveDownstream()
{
    if (Command::canUp == nullptr) {
        logger.PrintLog(Warn, "No downstream CAN device found");
        return false;
    }

    Command::current = Command();
    if (canDown->Receive(Command::current) == 0) {
        return false;
    }

    Command::current.Load();

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
    uint32_t pgn = (id >> PGN_POSITION) & PGN_SIZE;

    target = (pgn >> PGN_CMD_TARGET_IDX) & PGN_CMD_TARGET;
    type = static_cast<cmd_type_t>((pgn >> PGN_CMD_TYPE_IDX) & PGN_CMD_TYPE);
    arg = (pgn >> PGN_CMD_ARG_IDX) & PGN_CMD_ARG;

    if (type == Calibrate && (arg & PGN_CAL_CONF_M1 || arg & PGN_CAL_CONF_M2)) {
        mass = unpackFloatFromData(data);
    } else {
        mass = 0;
    }
}
