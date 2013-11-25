#include "uart.h"


/**
 * This sends a String over UART.
 * @param sToSend the string to send.
 */
void uart_send(char* sToSend)
{
	while (*sToSend)
	{
		while (!(UCSRA & (1 << UDRE)))
		{
		};
		UDR = *sToSend;
		sToSend++;
	}
	while (!(UCSRA & (1 << UDRE)))
	{
	};
	UDR = '\n';
}

/**
 * this sends one 8-bit character
 * @param sToSend the char to send.
 */
void uart_send_char(unsigned char sToSend)
{

	while (!(UCSRA & (1 << UDRE))){};
	UDR = sToSend;
}

/**
 * Initialize the UART for async 8N1 transmission.
 */
void uart_init(void)
{

	UCSRB |= (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);  // UART RX, TX und RX Interrupt einschalten
	UCSRC |= (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0); // Asynchron 8N1

	UBRRH = UBRR_VAL >> 8;
	UBRRL = UBRR_VAL & 0xFF;
}
