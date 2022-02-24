/*
 * update.c
 *
 * Created: 2022-02-24 13:47:56
 *  Author: Jeremy
 */ 

#include <tbd.h>

uint8_t updateSendDownstream(message_t *update)
{
	return CANSend(Down, update->id, update->length, (uint8_t *)update->data);
}

uint8_t updateReceiveUpstream(message_t *update)
{
	return CANReceive(Up, &update->id, &update->length, (uint8_t **)update->data);
}

uint8_t updateHandle(message_t *update)
{
	++(update->data[TBD_UPDATE_TANK_ID_IDX]);  // TODO
	return 1;
}

