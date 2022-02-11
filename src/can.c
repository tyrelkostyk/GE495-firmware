/*
 * can.c
 *
 * Created: 2022-02-01 16:42:18
 *  Author: Jeremy
 */ 

#include <tbd.h>

#include <asf.h>

extern uint32_t systemClk;

can_mb_conf_t can_mbox_down_tx;
can_mb_conf_t can_mbox_down_rx;
can_mb_conf_t can_mbox_up_tx;
can_mb_conf_t can_mbox_up_rx;

uint8_t CANSetup(void)
{
	/*** CAN0 is downstream, CAN1 is upstream ***/
	
	pmc_enable_periph_clk(ID_CAN0);
	pmc_enable_periph_clk(ID_CAN1);
	
	// If either call to can_init() fails, there is a bigger problem with the config
	// or with the device itself and we need to abort
	if (!can_init(TBD_CAN_DOWN, systemClk, TBD_CAN_BAUD))
		return 0;
	if (!can_init(TBD_CAN_UP, systemClk, TBD_CAN_BAUD))
		return 0;
	
	can_reset_all_mailbox(TBD_CAN_DOWN);
	can_reset_all_mailbox(TBD_CAN_UP);
	
	can_mbox_down_tx.ul_mb_idx = TBD_CAN_TX_IDX;
	can_mbox_down_tx.uc_obj_type = CAN_MB_TX_MODE;
	can_mbox_down_tx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_down_tx.ul_id = CAN_MID_MIDvA(TBD_CAN_DOWN_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_tx);
	
	can_mbox_down_rx.ul_mb_idx = TBD_CAN_RX_IDX;
	can_mbox_down_rx.uc_obj_type = CAN_MB_RX_MODE;  // Receive, no overwrite
	can_mbox_down_rx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_down_rx.ul_id = CAN_MID_MIDvA(TBD_CAN_DOWN_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_rx);
	
	can_mbox_up_tx.ul_mb_idx = TBD_CAN_TX_IDX;
	can_mbox_up_tx.uc_obj_type = CAN_MB_TX_MODE;
	can_mbox_up_tx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_up_tx.ul_id = CAN_MID_MIDvA(TBD_CAN_UP_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_tx);
	
	can_mbox_up_rx.ul_mb_idx = TBD_CAN_RX_IDX;
	can_mbox_up_rx.uc_obj_type = CAN_MB_RX_MODE;
	can_mbox_up_rx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_up_rx.ul_id = CAN_MID_MIDvA(TBD_CAN_UP_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_rx);

	can_enable_interrupt(TBD_CAN_UP, 0);
	can_enable_interrupt(TBD_CAN_DOWN, 0);

	NVIC_EnableIRQ(CAN0_IRQn);
	NVIC_EnableIRQ(CAN1_IRQn);
	
	return 1;
}

uint8_t CANSend(direction_t direction, uint32_t id, uint8_t length, const uint8_t *data)
{
	Can *can_module;
	can_mb_conf_t *mbox;
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
			return 0;
	}

	mbox->ul_id = CAN_MID_MIDvA(id) | CAN_MID_MIDvB(id);
	for (int i = 0; i < 4; i++) {
		mbox->ul_datal |= data[i] << (8*i);
		mbox->ul_datah |= data[i+4] << (8*i);
	}
	mbox->uc_length = length;

	uint32_t status;
	while ((status = can_mailbox_write(can_module, mbox)) != CAN_MAILBOX_TRANSFER_OK) { }
	if (status == CAN_MAILBOX_TRANSFER_OK) {
		can_mailbox_send_transfer_cmd(can_module, mbox);
		return 1;
	} else {
		return 0;
	}

}

uint8_t CANReceive(direction_t direction, uint32_t *id, uint8_t **data)
{
	Can *can_module;
	can_mb_conf_t *mbox;
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
			return 0;
	}
	
	if (!(can_mailbox_get_status(can_module, TBD_CAN_RX_IDX) & CAN_MSR_MRDY))
		return 0;
	
	uint32_t status;
	if ((status = can_mailbox_read(can_module, mbox)) & CAN_MAILBOX_TRANSFER_OK) {
		// Successful read
		*id = mbox->ul_id;  // TODO does this need to be converted using CAN_MID_MIDvA/B?
		for (int i = 0; i < mbox->uc_length; i++) {
			*data[i] = mbox->ul_datal & (0xff << 8*i);
			*data[i+4] = mbox->ul_datah & (0xff << 8*i);
		}
		return 1;
	} else if (status & CAN_MAILBOX_RX_NEED_RD_AGAIN) {
		// TODO Not sure what to do here - this indicates that a message was ignored or overridden
		return 0;  // For now
	}

	return 0;
	
}
