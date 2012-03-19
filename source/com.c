/*
 * com_neu.c
 *
 *  Created on: 19.03.2012
 *      Author: gabm
 */
#include "com.h"

void com_open_port(const char cDirection)
{
	adr_set(NET_ADR,3);
	BUS_DDR = cDirection;
	BUS_PORT = ~cDirection;
}

void com_close_port()
{
	//configure bus input & pull ups
	BUS_DDR = 0x00;
	BUS_PORT = 0xFF;

	adr_set(0x00, 3);

	bus_input();
}

void com_init()
{
	DDRC = 0x00;
	PORTC = 0xFF;
}


void com_write_byte(const char cData, uint8_t uiStatus)
{
	//clear last 4 bytes
	BUS_PORT &= ~(0x0F);

	//write high nibble
	BUS_PORT |= 0x0F & (cData >> 4);

	//signal that nibble is valid
	sbi(OUT_SIG_PORT,OUT_SIG_BYTE);

	//wait for ack and clear valid mark
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_BYTE);
	cbi(OUT_SIG_PORT,OUT_SIG_BYTE);

	//if this is the last bit, show it to the slave
	if (uiStatus == STATUS_LAST_BYTE)
		cbi(OUT_SIG_PORT,OUT_SIG_CHUNK);

	//clear output
	BUS_PORT &= ~(0x0F);

	//write low nibbles
	BUS_PORT |= 0x0F & cData;

	//signal that nibble is valid
	sbi(OUT_SIG_PORT, OUT_SIG_BYTE);

	//wait for ack and clear valid mark
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_BYTE);
	cbi(OUT_SIG_PORT, OUT_SIG_BYTE);
}

char com_read_byte()
{
	char result = 0x00;

	//wait for signal of new byte
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_BYTE);

	//first receive high nibble
	result = (( 0x0F & BUS_PIN) << 4);

	//indicate we received it
	sbi(OUT_SIG_PORT, OUT_SIG_BYTE);

	//wait for low bit PC6, indicating the next nibble is on the way
	loop_until_bit_is_clear(IN_SIG_PIN, IN_SIG_BYTE);

	cbi(OUT_SIG_PORT, OUT_SIG_BYTE);

	//wait for next nibble
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_BYTE);

	result |= (0x0F & PINC);

	//indicate we received it
	sbi(OUT_SIG_PORT, OUT_SIG_BYTE);

	//wait for low bit, indicating the byte is complete
	loop_until_bit_is_clear(IN_SIG_PIN, IN_SIG_BYTE);

	cbi(OUT_SIG_PORT,OUT_SIG_BYTE);

	return result;
}

void com_send_chunk(uint8_t uiLen)
{
	//indicate starting chunk & wait for begin
	sbi(OUT_SIG_PORT,OUT_SIG_CHUNK);
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_CHUNK);

	unsigned char* itr = com_send_buffer;
	uint8_t status = STATUS_NORMAL;
	for (uint8_t i =0; i<uiLen;i++)
	{
		if(i == uiLen-1)
			status = STATUS_LAST_BYTE;
		com_write_byte(*itr, status);
		itr++;

	}
	loop_until_bit_is_clear(IN_SIG_PIN, IN_SIG_CHUNK);
}

void com_receive_chunk( uint8_t* uiLen )
{
	unsigned char* itr = com_receive_buffer;

	//wait for begin & indicate readiness
	loop_until_bit_is_set(IN_SIG_PIN, IN_SIG_CHUNK);
	sbi(OUT_SIG_PORT,OUT_SIG_CHUNK);

	//receive max num chunks
	uint8_t i=0;
	while(i<MAX_CHUNK_SIZE)
	{

		*itr++ = com_read_byte();
		i++;
		if (bit_is_clear(IN_SIG_PIN, IN_SIG_CHUNK))
			break;

	}

	cbi(OUT_SIG_PORT,OUT_SIG_CHUNK);
	*uiLen = i;
}

void com_receive(uint8_t* uiLen)
{
	com_open_port(BUS_INPUT);
	com_receive_chunk(uiLen);
	com_close_port();
}

void com_send(uint8_t uiLen)
{
	com_open_port(BUS_OUTPUT);
	com_send_chunk(uiLen);
	com_close_port();
}
