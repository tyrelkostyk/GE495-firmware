//
// Created by jeremy on 2022-03-22.
//

#include "Message.h"


CanModule *Message::canUp = nullptr;
CanModule *Message::canDown = nullptr;


void Message::Init(CanModule *up, CanModule *down)
{
    Message::canUp = up;
    Message::canDown = down;
}
