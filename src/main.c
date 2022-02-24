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

//#if BOARD == SAM4E_XPLAINED_PRO
	ioport_set_pin_mode(PIN_CAN1_RX_IDX, PIN_CAN1_RX_FLAGS);
	// ioport_disable_pin(PIN_CAN1_RX_IDX);
	ioport_set_pin_mode(PIN_CAN1_TX_IDX, PIN_CAN1_TX_FLAGS);
	// ioport_disable_pin(PIN_CAN1_TX_IDX);

	/* Configure the transiver1 RS & EN pins. */
	ioport_set_pin_dir(PIN_CAN1_TR_RS_IDX, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(PIN_CAN1_TR_EN_IDX, IOPORT_DIR_OUTPUT);

//#endif

	systemClk = sysclk_get_main_hz();
	board_init();

	CANSetup();

	while (1) {
		// Check to see if it's time to sample the ADC output
		
		// Check if it's time to send an update
		
		// Poll for commands from downstream and respond accordingly
		if (cmdReceiveDownstream(&currentCommand) != 0) {
			cmdSendUpstream(&currentCommand);
			// Execute command on this node
		}
		
		// Poll for updates from upstream and respond accordingly
		if ((CANReceive(Up, &currentUpdate.id, (uint8_t **)&currentUpdate.data)) != 0) {
			x--;
		}
		
	}
	
}
