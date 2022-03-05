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

uint8_t cmdHandle(message_t *command)
{
	// TODO
	// - read command->id
	// - extract PGN
	// - determine if PGN is useful for this device
	// - if so, convert data appropriately and call the appropriate function
	uint32_t PGN = 0;  // TODO create and call PGN extraction function
	
	switch (PGN) {
		// TODO
		default: {
			break;
		}
	}

	return 1;
}

