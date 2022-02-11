/*
 * tbd.h
 *
 * Created: 2022-02-01 18:42:16
 *  Author: Jeremy
 */ 


#ifndef TBD_H_
#define TBD_H_

#include <stdint.h>

typedef struct {
	uint32_t id;
	uint8_t data[8];
} message_t;

/*** CAN ***/
#define TBD_CAN_BAUD CAN_BPS_500K

#define TBD_CAN_DOWN CAN0
#define TBD_CAN_UP   CAN1
typedef enum {
	Up,
	Down
} direction_t;

#define TBD_CAN_TX_IDX (0)
#define TBD_CAN_RX_IDX (1)

#define TBD_CAN_DOWN_ADDR (0x01)
#define TBD_CAN_UP_ADDR   (0x02)

uint8_t CANSetup(void);
uint8_t CANSend(direction_t direction, uint32_t id, uint8_t length, const uint8_t *data);
uint8_t CANReceive(direction_t direction, uint32_t *id, uint8_t **data);


#endif /* TBD_H_ */