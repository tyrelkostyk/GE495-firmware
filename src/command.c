/*
 * command.c
 *
 * Created: 2022-02-24 13:27:56
 *  Author: Jeremy
 */ 

#include <tbd.h>

uint8_t cmdSendUpstream(message_t *command)
{
	return CANSend(Up, command->id, command->length, (uint8_t *)command->data);
}

uint8_t cmdReceiveDownstream(message_t *command)
{
	return CANReceive(Down, &command->id, &command->length, (uint8_t **)&command->data);
}

