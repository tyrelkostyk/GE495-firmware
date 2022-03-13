/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef DIGITIZER_H
#define DIGITIZER_H

#include <conf_board.h>


/***************************************************************************************************
                                         BOARD DEFINITIONS
***************************************************************************************************/

/** Name of the board */
#define BOARD_NAME "DIGITIZER"
/** Family definition (already defined) */
#define sam4e
/** Core definition */
#define cortexm4


/***************************************************************************************************
                                          CLOCK SETTINGS
***************************************************************************************************/

// External oscillator settings.
// Uncomment and set correct values if external oscillator is used.

// External oscillator frequency
//#define BOARD_XOSC_HZ          8000000

// External oscillator type.
//!< External clock signal
//#define BOARD_XOSC_TYPE        XOSC_TYPE_EXTERNAL
//!< 32.768 kHz resonator on TOSC
//#define BOARD_XOSC_TYPE        XOSC_TYPE_32KHZ
//!< 0.4 to 16 MHz resonator on XTALS
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL

// External oscillator startup time
//#define BOARD_XOSC_STARTUP_US  500000

// TODO: these are default values... are they correct?

/** Slow clock settings */
#  define BOARD_FREQ_SLCK_XTAL      (32768UL)
#  define BOARD_FREQ_SLCK_BYPASS    (32768UL)

/** Main clock settings */
#  define BOARD_FREQ_MAINCK_XTAL    (12000000UL)
#  define BOARD_FREQ_MAINCK_BYPASS  (12000000UL)

/** Board main clock start-up time */
#  define BOARD_OSC_STARTUP_US      (15625UL)

/** Master clock frequency */
#define BOARD_MCK                       CHIP_FREQ_CPU_MAX


/***************************************************************************************************
                                          PIN MACROS - CAN
***************************************************************************************************/

/** CAN0 (Downstream) PIN RX. */
#define PIN_CAN0_RX_IDX           PIO_PB3_IDX
#define PIN_CAN0_RX_FLAGS         IOPORT_MODE_MUX_A

/** CAN0 (Downstream) PIN TX. */
#define PIN_CAN0_TX_IDX           PIO_PB2_IDX
#define PIN_CAN0_TX_FLAGS         IOPORT_MODE_MUX_A

/** CAN1 (Upstream) PIN RX. */
#define PIN_CAN1_RX_IDX           PIO_PC12_IDX
#define PIN_CAN1_RX_FLAGS         IOPORT_MODE_MUX_C

/** CAN1 (Upstream) PIN TX. */
#define PIN_CAN1_TX_IDX           PIO_PC15_IDX
#define PIN_CAN1_TX_FLAGS         IOPORT_MODE_MUX_C


/***************************************************************************************************
                                          PIN MACROS - ADC
***************************************************************************************************/

// TODO: pull in pin macros for ADC control

#endif // DIGITIZER_H
