/*
 * com_neu.c
 *
 *  Created on: 19.03.2012
 *      Author: gabm
 */
#include "com.h"

volatile unsigned char cDataReq =0;
unsigned char com_receive_buffer[MAX_CHUNK_SIZE];
unsigned char com_send_buffer[MAX_CHUNK_SIZE];

void com_wait_low()
{
	loop_until_bit_is_clear(COM_PIN, COM_SCK_IN);
}

void com_wait_high()
{
	loop_until_bit_is_set(COM_PIN, COM_SCK_IN);
}

void com_send_ack()
{
	cbi(COM_PORT,COM_SCK_OUT);
	com_wait_high();
	sbi(COM_PORT,COM_SCK_OUT);
}
void com_mark_active_and_wait_ack()
{
	cbi(COM_PORT,COM_SCK_OUT);
	com_wait_low();
	sbi(COM_PORT, COM_SCK_OUT);
	com_wait_high();
}

void com_set_command(unsigned char cCommand)
{

	CMD_PORT = (CMD_PORT & ~(0xC0)) | (cCommand << 6);

	com_mark_active_and_wait_ack();


	//after receive set to idle
	CMD_PORT = (CMD_PORT & ~(0xC0)) | 0x00;
}

void com_connect()
{
	// set address
	sys_adr_set(COM_ADR,3);

	//SCK IN as input and pull up
	cbi(COM_DDR, COM_SCK_IN);
	sbi(COM_PORT, COM_SCK_IN);

	//SCK out as output and high
	sbi(COM_DDR, COM_SCK_OUT);
	sbi(COM_PORT, COM_SCK_OUT);

	//CMD as output and low
	sbi(CMD_DDR, CMD_PIN_0);
	cbi(CMD_PORT, CMD_PIN_0);

	sbi(CMD_DDR, CMD_PIN_1);
	cbi(CMD_PORT, CMD_PIN_1);

	com_config(BUS_INPUT);


	com_mark_active_and_wait_ack();


}

void com_disconnect()
{
	sys_adr_set(0,3);
	COM_DDR = 0x00;
	COM_PORT = 0xFF;

	cbi(CMD_DDR, CMD_PIN_0);
	sbi(CMD_PORT, CMD_PIN_0);

	cbi(CMD_DDR, CMD_PIN_1);
	sbi(CMD_PORT, CMD_PIN_1);
	_delay_ms(10);
}

void com_config(char cDirection)
{
	if (cDirection == BUS_OUTPUT)
	{
		COM_DDR |= 0x0F;
		COM_PORT &= ~(0x0F);
	} else {
		COM_DDR &= ~(0x0F);
		COM_PORT |= 0x0F;
	}
}

void com_init()
{
	DDRC = 0x00;
	PORTC = 0xFF;

	//external interrupt on falling edge
	cbi(MCUCR,ISC00);
	sbi(MCUCR,ISC01);

	//set int pin as input
	sbi(COM_INT_PORT,COM_INT_PIN);
	cbi(COM_INT_DDR,COM_INT_PIN);

	//enable external interrupt 1
	sbi(GICR,INT0);
}

ISR(INT0_vect)
{
	cDataReq =1;
	cbi(GICR,INT0);
}

void com_write_nibble(const unsigned char cData)
{
	COM_PORT = (COM_PORT & 0xF0) | (0x0F & cData);

	com_mark_active_and_wait_ack();
}

unsigned char com_read_nibble()
{
	unsigned char cResult = 0x00;
	com_wait_low();
	cResult |= (0x0F & COM_PIN);
	com_send_ack();
	return cResult;
}

void com_write_byte(const unsigned char cData)
{
	com_write_nibble(cData >> 4);
	com_write_nibble(cData);
}

unsigned char com_read_byte()
{
	unsigned char cResult = 0x00;
	cResult |= (com_read_nibble() << 4);
	cResult |= com_read_nibble();
	return cResult;
}

void com_send_chunk(uint8_t uiLen)
{
	unsigned char* itr = com_send_buffer;
	com_write_byte(uiLen);

	for (uint8_t i =0; i<uiLen;i++)
	{
		com_write_byte(*itr);
		itr++;
	}
}

void com_receive_chunk( uint8_t* uiLen)
{
	//init and read first byte
	unsigned char* itr = com_receive_buffer;
	*uiLen = com_read_byte();

	//receive the proposed length of bytes
	uint8_t i=0;
	while(i < *uiLen)
	{
		*itr++ = com_read_byte();
		i++;
	}
}

char com_check()
{
	return cDataReq;
}
