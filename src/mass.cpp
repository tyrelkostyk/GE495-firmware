// mass.c
// Contains functions for converting raw data values to an appropriate mass unit.

#include "defs.h"

uint32_t currentMass;

void massGetCurrent(uint8_t *buffer)
{
    // Split 32-bit number into 8-bit chunks, little-endian
    for (int i = 0; i < MASS_NUM_BYTES; i++) {
        buffer[i] = (currentMass >> 8*i) & 0xff;
    }
}

