/*
 * tbd.h
 *
 * Created: 2022-02-01 18:42:16
 *  Author: Jeremy
 */


#ifndef TBD_H_
#define TBD_H_

#include <stdint.h>
#include <board.h>


/*** CAN ***/
#define TBD_CAN_BAUD CAN_BPS_250K

#define TBD_CAN_DOWN CAN0
#define TBD_CAN_UP   CAN1

typedef enum {
	Up,
	Down
} direction_t;

#define CAN_MAX_PAYLOAD_SIZE	(8)

typedef struct {
	uint32_t id;
	uint8_t length;
	uint8_t data[CAN_MAX_PAYLOAD_SIZE];
} message_t;

#define TBD_CAN_TX_IDX (0)
#define TBD_CAN_RX_IDX (1)

#define TBD_CAN_DOWN_ADDR (0x01)
#define TBD_CAN_UP_ADDR   (0x02)

uint8_t canInit(void);
uint8_t canTransmit(direction_t direction, uint32_t id, uint8_t length, const uint8_t *data);
uint8_t canReceive(direction_t direction, uint32_t *id, uint8_t *length, uint8_t **data);


/*** Update ***/
#define TBD_UPDATE_TANK_ID_IDX (0)

typedef struct {
	uint8_t tankId;
	float mass;
} update_t;

uint8_t updateSendDownstream(message_t *message);
uint8_t updateReceiveUpstream(message_t *message);
uint8_t updateCreate(message_t *message, float mass);
uint8_t updateHandle(message_t *message);


/*** Command ***/
uint8_t cmdSendUpstream(message_t *command);
uint8_t cmdReceiveDownstream(message_t *command);
uint8_t cmdHandle(message_t *command);


/*** ADC ***/
typedef enum _adcChannel_t {
	adcChannelZero	= 0,
	adcChannelOne	= 1,
	adcChannelTwo	= 2,
	adcChannelThree = 3,
} adcChannel_t;

void adcInit(void);
int32_t adcReadAllChannels(void);
int32_t adcReadAllSmooth(void);
int32_t adcReadChannelSmooth(adcChannel_t channel);
int32_t adcReadAndCalibrate(adcChannel_t channel);


/*** CALIBRATION ***/
int32_t calibrationOffset(adcChannel_t channel);
float calibrationConversionFactor(void);

void calibrationTare(adcChannel_t channel, int32_t offset);
void calibrationTareAllLoadCells(void);

void calibrationObtainMassOne(float mass);
void calibrationObtainMassTwo(float mass);


/*** GENERAL ***/
#define SUCCESS	(0)
#define FAILURE	(1)

#define LOAD_CELLS_PER_TANK	3

void delayInit(void);
void delayFor(uint32_t us);


#endif /* TBD_H_ */
