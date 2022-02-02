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
	
	can_mbox_down_tx.ul_mb_idx = 0;
	can_mbox_down_tx.uc_obj_type = CAN_MB_TX_MODE;
	can_mbox_down_tx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_down_tx.ul_id = CAN_MID_MIDvA(TBD_CAN_DOWN_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_tx);
	
	can_mbox_down_rx.ul_mb_idx = 1;
	can_mbox_down_rx.uc_obj_type = CAN_MB_RX_MODE;  // Receive, no overwrite
	can_mbox_down_rx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_down_rx.ul_id = CAN_MID_MIDvA(TBD_CAN_DOWN_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_DOWN, &can_mbox_down_rx);
	
	can_mbox_up_tx.ul_mb_idx = 0;
	can_mbox_up_tx.uc_obj_type = CAN_MB_TX_MODE;
	can_mbox_up_tx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_up_tx.ul_id = CAN_MID_MIDvA(TBD_CAN_UP_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_tx);
	
	can_mbox_up_rx.ul_mb_idx = 1;
	can_mbox_up_rx.uc_obj_type = CAN_MB_RX_MODE;
	can_mbox_up_rx.ul_id_msk = CAN_MAM_MIDvA_Msk | CAN_MAM_MIDvB_Msk;  // Extended ID
	can_mbox_up_rx.ul_id = CAN_MID_MIDvA(TBD_CAN_UP_ADDR);  // TODO: Find proper masks for these
	can_mailbox_init(TBD_CAN_UP, &can_mbox_up_rx);
	
	return 1;
}