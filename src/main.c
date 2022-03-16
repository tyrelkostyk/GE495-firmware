/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <asf.h>
#include <can.h>
#include <ioport.h>
#include <tbd.h>

extern can_mb_conf_t can_mbox_up_rx, can_mbox_up_tx;
extern can_mb_conf_t can_mbox_down_rx, can_mbox_down_tx;

uint32_t systemClk = 0;
uint32_t cpuClk = 0;

message_t currentCommand = { 0 };
message_t currentUpdate = { 0 };

uint8_t x = 0;

int main (void)
{
	// Initialize system clock
	sysclk_init();

	/* Board initialization should include the following:
	    - clock config
		- timer config
		- peripheral config
		- CAN settings
		- TWI settings
		- GPIO settings
	*/

	systemClk = sysclk_get_main_hz();
	cpuClk = sysclk_get_cpu_hz();

	// initialize the board settings
	board_init();

	serialSetup();

	// initialize the CAN peripheral
	CANSetup();

	// initialize delay functionality
	delayInit();

	// initialize the ADC
	adcInit();
	


	while (1) {

		// TODO: Check to see if it's time to sample the ADC output

		// Sample ADC output
		int32_t data = adcReadAllSmooth();
	
		/*** CALIBRATION ***/


		// TODO: Check if it's time to send an update

		// Poll for commands from downstream and respond accordingly
		if (cmdReceiveDownstream(&currentCommand) != 0) {
			if (!cmdSendUpstream(&currentCommand)) {
				// Something went wrong with the transmission of the command
			}
			if (!cmdHandle(&currentCommand)) {
				// Something went wrong with the handling of this command
			}
		}
				// Poll for updates from upstream and respond accordingly
		if (updateReceiveUpstream(&currentUpdate) != 0) {
			if (!updateHandle(&currentUpdate)) {
		 		// Something went wrong with the update handling
		 	}
		 	if (!updateSendDownstream(&currentUpdate)) {
		 		// Something went wrong with the transmission of the update
		 	}
		}

	}

}
