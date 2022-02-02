/*
 * tbd.h
 *
 * Created: 2022-02-01 18:42:16
 *  Author: Jeremy
 */ 


#ifndef TBD_H_
#define TBD_H_

#include <stdint.h>

/*** CAN ***/
#define TBD_CAN_BAUD CAN_BPS_500K

#define TBD_CAN_DOWN CAN0
#define TBD_CAN_UP   CAN1

#define TBD_CAN_DOWN_ADDR 0x01
#define TBD_CAN_UP_ADDR   0x02

uint8_t CANSetup(void);


#endif /* TBD_H_ */