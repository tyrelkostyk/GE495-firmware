/*
 * uart.c
 *
 * Created: 2022-03-16 11:57:45
 *  Author: Jeremy
 */ 

#include <tbd.h>
#include <asf.h>

static usart_serial_options_t usart_options = {
	.baudrate = US_BAUDRATE,
	.charlength = US_CHAR_LENGTH,
	.paritytype = US_PARITY_TYPE,
	.stopbits = US_STOP_BITS
};

void serialSetup(void)
{
	usart_serial_init(USART0, &usart_options);
}
