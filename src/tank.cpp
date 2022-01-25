// tank.c
// Contains tank ID association functions.

#include "defs.h"

uint8_t tankID = 0;

void tankRefreshID(uint8_t refID)
{
    tankID = (tankID <= refID) ? (refID + 1) : tankID;
}
