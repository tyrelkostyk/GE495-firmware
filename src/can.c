/*
 * can.c
 *
 * Created: 2022-02-01 16:42:18
 *  Author: Jeremy
 */ 

#include <tbd.h>

#include <asf.h>
#include <can.h>
#include <digitizer.h>
#include <string.h>


extern uint32_t systemClk;
extern uint32_t cpuClk;

volatile uint32_t g_can_up_receive_status = 0;
volatile uint32_t g_can_down_receive_status = 0;

can_mb_conf_t can_mbox_down_tx = { 0 };
can_mb_conf_t can_mbox_down_rx = { 0 };
can_mb_conf_t can_mbox_up_tx = { 0 };
can_mb_conf_t can_mbox_up_rx = { 0 };

// Downstream Receive Interrupt Handler
void CAN0_Handler(void)
{
	uint32_t status;
	status = can_mailbox_get_status(CAN0, TBD_CAN_RX_IDX);
	if ((status & CAN_MSR_MRDY) == CAN_MSR_MRDY) {
		can_mbox_up_rx.ul_mb_idx = TBD_CAN_RX_IDX;  // I don't know why or even if this is important
		can_mbox_up_rx.ul_status = status;
		can_mailbox_read(CAN0, &can_mbox_up_rx);
		g_can_down_receive_status = 1;
	}
}

// Upstream Receive Interrupt Handler
void CAN1_Handler(void)
{
	uint32_t status;
	status = can_mailbox_get_status(CAN1, TBD_CAN_RX_IDX);
	if ((status & CAN_MSR_MRDY) == CAN_MSR_MRDY) {
		can_mbox_up_rx.ul_mb_idx = TBD_CAN_RX_IDX;  // I don't know why or even if this is important
		can_mbox_up_rx.ul_status = status;
		can_mailbox_read(CAN1, &can_mbox_up_rx);
		g_can_up_receive_status = 1;
	}
}

uint8_t canInit(void)
{
	/*** CAN0 is downstream, CAN1 is upstream ***/

	// enable CAN0 (downstream) TX and RX pins
	ioport_set_pin_mode(PIN_CAN0_RX_IDX, PIN_CAN0_RX_FLAGS);
	ioport_set_pin_mode(PIN_CAN0_TX_IDX, PIN_CAN0_TX_FLAGS);

	// enable CAN1 (downstream) TX and RX pins
	ioport_set_pin_mode(PIN_CAN1_RX_IDX, PIN_CAN1_RX_FLAGS);
	ioport_set_pin_mode(PIN_CAN1_TX_IDX, PIN_CAN1_TX_FLAGS);
	
	// Initialize the CAN peripheral clock	
	pmc_enable_periph_clk(ID_CAN0);
	pmc_enable_periph_clk(ID_CAN1);

	// Initialize the CAN peripherals
	if (!can_init(TBD_CAN_DOWN, systemClk, TBD_CAN_BAUD))
		return FAILURE;
	if (!can_init(TBD_CAN_UP, systemClk, TBD_CAN_BAUD))
		return FAILURE;
	
	// Reset the CAN mailboxes
	can_reset_all_mailbox(TBD_CAN_DOWN);
	can_reset_all_mailbox(TBD_CAN_UP);
	
	// Configure the downstream transmit mailbox
	can_mbox_down_tx.ul_mb_idx = TBD_CAN_TX_IDX;				// Transmit Mailbox Index
	can_mbox_down_tx.uc_obj_type = CAN_MB_TX_MODE;				// Transmit Mode
	can_mbox_down_tx.uc_tx_prio = 15;							// Low priority
	can_mbox_down_tx.uc_id_ver = 1;								// Extended ID
	can_mbox_down_tx.ul_id_msk = 0;
	can_mbox_down_tx.ul_id = CAN_MID_MIDvB(TBD_CAN_DOWN_ADDR);	// TODO: determine proper ID (unique?)
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_tx);
	
	// Configure the downstream receive mailbox
	can_mbox_down_rx.ul_mb_idx = TBD_CAN_RX_IDX;				// Receive Mailbox Index
	can_mbox_down_rx.uc_obj_type = CAN_MB_RX_OVER_WR_MODE;		// Receive, and overwrite
	can_mbox_down_rx.uc_id_ver = 1;								// Extended ID
	//can_mbox_down_rx.ul_id_msk = 0;							// Accept all incoming IDs
	can_mbox_down_rx.ul_id_msk = CAN_MAM_MIDvB_Msk;				// Accept extended ID frames TODO: accept only Digitizer messages?
	can_mbox_down_rx.ul_id = CAN_MID_MIDvA(TBD_CAN_DOWN_ADDR);  // TODO: determine proper ID (unique?)
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_rx);

	// Configure the upstream transmit mailbox	
	can_mbox_up_tx.ul_mb_idx = TBD_CAN_TX_IDX;					// Transmit Mailbox Index
	can_mbox_up_tx.uc_obj_type = CAN_MB_TX_MODE;				// Transmit Mode
	can_mbox_up_tx.uc_tx_prio = 10;								// High priority
	can_mbox_up_tx.uc_id_ver = 1;								// Extended ID
	can_mbox_up_tx.ul_id_msk = 0;
	can_mbox_up_tx.ul_id = CAN_MID_MIDvA(TBD_CAN_UP_ADDR);		// TODO: determine proper ID (unique?)
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_tx);
	
	// Configure the upstream receive mailbox
	can_mbox_up_rx.ul_mb_idx = TBD_CAN_RX_IDX;					// Receive Mailbox Index
	can_mbox_up_rx.uc_obj_type = CAN_MB_RX_OVER_WR_MODE;		// Receive & Overwrite
	can_mbox_up_rx.uc_id_ver = 1;								// Extended ID
	//can_mbox_up_rx.ul_id_msk = 0;								// Accept all incoming IDs
	can_mbox_up_rx.ul_id_msk = CAN_MAM_MIDvB_Msk;				// Accept extended ID frames TODO: accept only Digitizer messages?
	can_mbox_up_rx.ul_id = CAN_MID_MIDvB(TBD_CAN_UP_ADDR);		// TODO: determine proper ID (unique?)
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_rx);

	// Enable CAN interrupts
	NVIC_EnableIRQ(CAN0_IRQn);
	NVIC_EnableIRQ(CAN1_IRQn);
	can_enable_interrupt(TBD_CAN_DOWN, (1<<TBD_CAN_RX_IDX));
	can_enable_interrupt(TBD_CAN_UP, (1<<TBD_CAN_RX_IDX));

	return SUCCESS;
}

uint8_t canTransmit(direction_t direction, uint32_t id, uint8_t length, const uint8_t *data)
{
	Can *can_module;
	can_mb_conf_t *mbox;
	
	// determine which direction we're transmitting to
	switch (direction) {
		case (Up):
			can_module = TBD_CAN_UP;
			mbox = &can_mbox_up_tx;
			break;
		case (Down):
			can_module = TBD_CAN_DOWN;
			mbox = &can_mbox_down_tx;
			break;
		default:
			return FAILURE;
	}

	// set the ID of the message
	mbox->ul_id = CAN_MID_MIDvB(TBD_CAN_UP_ADDR);	// TODO: change id
	
	// set the low and high (4 bytes each) message contents
	memcpy(&(mbox->ul_datal), data, sizeof(mbox->ul_datal));
	memcpy(&(mbox->ul_datah), &data[sizeof(mbox->ul_datal)], sizeof(mbox->ul_datah));
	
	// set the length of the message
	mbox->uc_length = length;

	// get the status of the transmit mailbox
	uint32_t write_status = can_mailbox_write(can_module, mbox);

	// get the status of the CAN peripheral
	uint32_t status = can_get_status(can_module);

	// transmit the message in the mailbox if it's ready
	if (write_status == CAN_MAILBOX_TRANSFER_OK) {
		can_mailbox_send_transfer_cmd(can_module, mbox);
		return SUCCESS;
	}

	return FAILURE;
}

uint8_t canReceive(direction_t direction, uint32_t *id, uint8_t *length, uint8_t **data)
{
	Can *can_module;
	can_mb_conf_t *mbox;
	
	// determine which direction we're receiving from
	switch (direction) {
		case (Up):
			can_module = TBD_CAN_UP;
			mbox = &can_mbox_up_rx;
			break;
		case (Down):
			can_module = TBD_CAN_DOWN;
			mbox = &can_mbox_down_rx;
			break;
		default:
			return FAILURE;
	}
	
	// poll to see if there is a CAN message in the receive mailbox
	if (!(can_mailbox_get_status(can_module, TBD_CAN_RX_IDX) & CAN_MSR_MRDY))
		return FAILURE;

	// read the available message	
	uint32_t status = can_mailbox_read(can_module, mbox);

	// extract the CAN message properties
	if (status & CAN_MAILBOX_TRANSFER_OK) {
		// grab the CAN message ID
		*id = mbox->ul_id;
		// grab the CAN message payload length
		*length = mbox->uc_length;
		// grab the CAN message payload
		memcpy(*data, &mbox->ul_datal, sizeof(mbox->ul_datal));
		memcpy(&((*data)[sizeof(mbox->ul_datal)]), &mbox->ul_datah, sizeof(mbox->ul_datah));
		return SUCCESS;
	}
	
	else if (status & CAN_MAILBOX_RX_NEED_RD_AGAIN) {
		// TODO Not sure what to do here - this indicates that a message was ignored or overridden. Do we care?
		return FAILURE;  // For now
	}

	return FAILURE;
	
}
