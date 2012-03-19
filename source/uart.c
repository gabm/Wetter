#include "uart.h"

#define UART_BUFFER_SIZE 10
volatile char cBuffer[UART_BUFFER_SIZE];
volatile uint8_t uiCurrent;
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


	uiCurrent = 0;

	sei();

}
static void uart_parse_req()
{
	uart_send("Command..");
	char cReceived [UART_BUFFER_SIZE];
	for (uint8_t i=0;i<UART_BUFFER_SIZE;i++)
	{
		cReceived[i] = cBuffer[i];
		cBuffer[i] = 0x00;
	}

	if (strcmp(cReceived,"print0") == 0x00) mem_print_sector(0x00);
	if (strcmp(cReceived,"print1")== 0x00) mem_print_sector(0x01);
	if (strcmp(cReceived,"print2")== 0x00) mem_print_sector(0x02);
	if (strcmp(cReceived,"print3")== 0x00) mem_print_sector(0x03);
	if (strcmp(cReceived,"print4")== 0x00) mem_print_sector(0x04);
	if (strcmp(cReceived,"print5")== 0x00) mem_print_sector(0x05);
	if (strcmp(cReceived,"print6")== 0x00) mem_print_sector(0x06);
	if (strcmp(cReceived,"print7")== 0x00) mem_print_sector(0x07);

	if (strcmp(cReceived,"clear0")== 0x00) mem_sector_erase(0x00);
	if (strcmp(cReceived,"clear1")== 0x00) mem_sector_erase(0x01);
	if (strcmp(cReceived,"clear2")== 0x00) mem_sector_erase(0x02);
	if (strcmp(cReceived,"clear3")== 0x00) mem_sector_erase(0x03);
	if (strcmp(cReceived,"clear4")== 0x00) mem_sector_erase(0x04);
	if (strcmp(cReceived,"clear5")== 0x00) mem_sector_erase(0x05);
	if (strcmp(cReceived,"clear6")== 0x00) mem_sector_erase(0x06);
	if (strcmp(cReceived,"clear7")== 0x00) mem_sector_erase(0x07);

	uiCurrent =0;
}
ISR(USART_RXC_vect)
{
	char cCurrentChar = UDR;
	if (cCurrentChar != '\n')
	{
		cBuffer[uiCurrent] = cCurrentChar;
		uiCurrent = (uiCurrent+1) % UART_BUFFER_SIZE;
	} else {
		uart_parse_req();
	}
}
