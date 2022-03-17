/**
 * @file main.c
 * Application entry point.
 */

#include <asf.h>
#include <can.h>
#include <ioport.h>
#include <tbd.h>

uint32_t systemClk = 0;
uint32_t cpuClk = 0;

message_t command = { 0 };
message_t localUpdate = { 0 };
message_t remoteUpdate = { 0 };


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
	if (canInit() != SUCCESS) {
		error++;
	}

	// initialize delay functionality
	delayInit();

	// initialize the ADC
	adcInit();

	//calibrationTareAllLoadCells();
	
	//calibrationObtainMassOne(2.5);
	
	//calibrationObtainMassTwo(7.5);

	uint32_t updatePeriod = 15 * 1000;
	uint32_t loopTimer = 0;

	int yay = 0;

	while (1) {
		
		loopTimer++;

		/*** UPDATE ACQUISITION & TRANSMISSION ***/

		// Check to see if it's time to sample the ADC output
		// TODO: use an accurate timer
		if ((loopTimer % updatePeriod) == 0) {
			
			// Obtain ADC output
			mass = (float) (adcReadAllSmooth() * calibrationConversionFactor());

			// Create the new update message
			updateCreate(&localUpdate, mass);
		
			// Transmit the new update message
			if (updateSendDownstream(&localUpdate) != SUCCESS) {
				// Something went wrong with the transmission of the update
				error++;
			} else {
				yay++;
			}
		}


		/*** CALIBRATION COMMANDS ***/

		// Poll for commands from downstream
		if (cmdReceiveDownstream(&command) != SUCCESS) {
			
			// Forward the commands upstream
			if (cmdSendUpstream(&command) != SUCCESS) {
				// Something went wrong with the transmission of the command
				error++;
			}
			
			// Handle the command
			if (cmdHandle(&command) != SUCCESS) {
				// Something went wrong with the handling of this command
				error++;
			}
		}
	
		
		/*** UPDATE FORWARDING ***/

		// Poll for upstream updates
		if (updateReceiveUpstream(&remoteUpdate) == SUCCESS) {
			
			// Handle the update
			if (updateHandle(&remoteUpdate) != SUCCESS) {
		 		// Something went wrong with the update handling
				error++;	
		 	}
			 
			// Forward the update downstream (if handling succeeded)
			else if (updateSendDownstream(&remoteUpdate) != SUCCESS) {
				// Something went wrong with the transmission of the update
				error++;
			}
		}
	}
}
