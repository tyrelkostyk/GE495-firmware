// can.c
// Contains low-level CAN-specific communication functions, including configuration.

#ifdef _ARDUINO
#include "Serial_CAN_Module.h"

Serial_CAN can;
#endif

#include "defs.h"

void CANSetup(void)
{
#ifdef Arduino_h
    can.begin(CAN_TX, CAN_RX, CAN_BAUDRATE);
#endif
}

void CANSend(uint32_t id, uint8_t ext, uint8_t rtr_bit, uint8_t length, const uint8_t *data)
{
#ifdef Arduino_h
    can.send(id, ext, rtr_bit, length, data);
#endif
}
