/**
 * @file main.c
 * Application entry point.
 */

#include <asf.h>
#include <can.h>
#include <ioport.h>
#include <tbd.h>

extern can_mb_conf_t can_mbox_up_rx, can_mbox_up_tx;
extern can_mb_conf_t can_mbox_down_rx, can_mbox_down_tx;

uint32_t systemClk = 0;
uint32_t cpuClk = 0;

message_t command = { 0 };
message_t localUpdate = { 0 };
message_t remoteUpdate = { 0 };

uint8_t x = 0;

int main (void)
{
	int error = 0;
	float mass = 0.0;
	
	// Initialize system clock
	sysclk_init();

	// get system and CPU clock settings
	systemClk = sysclk_get_main_hz();
	cpuClk = sysclk_get_cpu_hz();

	// initialize the board settings
	board_init();

	// initialize the CAN peripheral
	if (CANSetup() != SUCCESS) {
		error++;
	}

	// initialize delay functionality
	delayInit();

	// initialize the ADC
	adcInit();

	uint32_t updatePeriod = 100 * 1000;
	uint32_t loopTimer = 0;

	int yay_rx = 0;
	int yay_tx = 0;

	while (1) {
		
		loopTimer++;

		/*** UPDATE ACQUISITION & TRANSMISSION ***/

		// Check to see if it's time to sample the ADC output
		// TODO: use an accurate timer
		if ((loopTimer % updatePeriod) == 0) {
			
			// Obtain ADC output
			mass = (float) adcReadAllSmooth();

			// Create the new update message
			updateCreate(&localUpdate, mass);
		
			// Transmit the new update message
			if (updateSendDownstream(&localUpdate) != SUCCESS) {
				// Something went wrong with the transmission of the update
				error++;
			} else {
				yay_tx++;
			}
		}


		/*** CALIBRATION COMMANDS ***/

		/*
		// Poll for commands from downstream and respond accordingly
		if (cmdReceiveDownstream(&command) != 0) {
			if (!cmdSendUpstream(&command)) {
				// Something went wrong with the transmission of the command
				error++;
			}
			if (!cmdHandle(&command)) {
				// Something went wrong with the handling of this command
				error++;
			}
		}
		*/
		
		
		/*** UPDATE FORWARDING ***/

		// Poll for upstream updates
		if (updateReceiveUpstream(&remoteUpdate) == SUCCESS) {
			
			yay_rx++;
			
			// Handle the update
			if (updateHandle(&remoteUpdate) != SUCCESS) {
		 		// Something went wrong with the update handling
				error++;
				
		 	}
			 
			// Forward the update downstream
			else if (updateSendDownstream(&remoteUpdate) != SUCCESS) {
				// Something went wrong with the transmission of the update
				error++;
			}
		}
	}
}
