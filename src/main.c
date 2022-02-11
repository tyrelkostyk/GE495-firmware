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

extern can_mb_conf_t can_mbox_up_rx, can_mbox_up_tx;
extern can_mb_conf_t can_mbox_down_rx, can_mbox_down_tx;

uint32_t systemClk;

message_t currentCommand;
message_t currentUpdate;

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
	board_init();

	while (1) {
		// Check to see if it's time to sample the ADC output
		
		// Check if it's time to send an update
		
		// Poll for commands from downstream and respond accordingly
		if ((CANReceive(Down, &currentCommand.id, &currentCommand.data)) != 0) {
			x++;
		}
		
		// Poll for updates from upstream and respond accordingly
		if ((CANReceive(Up, &currentUpdate.id, &currentUpdate.data)) != 0) {
			x--;
		}
		
	}
	
}
