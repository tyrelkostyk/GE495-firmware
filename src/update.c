/*
 * update.c
 *
 * Created: 2022-02-24 13:47:56
 *  Author: Jeremy
 */ 

#include <tbd.h>
#include <string.h>


uint8_t updateSendDownstream(message_t *message)
{
	return CANSend(Down, message->id, message->length, (uint8_t *)message->data);
}

uint8_t updateReceiveUpstream(message_t *message)
{
	return CANReceive(Up, &message->id, &message->length, (uint8_t **)message->data);
}

uint8_t updateCreate(message_t *message, float mass)
{
	// Ensure message pointer is valid
	if (message == NULL)
		return FAILURE;
	
	// Clear previous contents of the message
	memset(message, 0, sizeof(message_t));
	
	// Create a new update
	update_t update = { 0 };
		
	// Initial ID of the message is 1		
	update.tankId = 1;
	
	// Set the mass
	update.mass = mass;
	
	// Copy the update into the message payload
	memcpy(message->data, &update, sizeof(update_t));
	
	// Set the length of the message
	message->length = sizeof(update_t);
	
	// Set the ID of the message
	message->id = 1;
	
	return SUCCESS;
}

uint8_t updateHandle(message_t *message)
{
	// Increment the tank ID field
	((update_t*)(message->data))->tankId++;
	return SUCCESS;
}

