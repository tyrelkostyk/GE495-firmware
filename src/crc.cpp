//
// Created by jeremy on 2022-03-06.
//

#include "defs.h"


uint16_t crc16MCRFXX(uint16_t crc, uint8_t *data, uint8_t len)
{
    uint8_t t;
    uint8_t L;
    if (!data || len <= 0)
        return crc;

    while (len--) {
        crc ^= *data++;
        L = crc ^ (crc << 4);
        t = (L << 3) | (L >> 5);
        L ^= (t & 0x07);
        t = (t & 0xf8) ^ (((t << 1) | (t >> 7)) & 0x0f) ^ (uint8_t)(crc >> 8);
        crc = (L << 8) | t;
    }
    return crc;
}
