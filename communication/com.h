/*
 * com_neu.h
 *
 *  Created on: 19.03.2012
 *      Author: gabm
 */

#ifndef COM_H_
#define COM_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../util.h"
#include "../core/system.h"
#include "../core/buffers.h"

#define COM_DDR DDRC
#define COM_PORT PORTC
#define COM_PIN PINC

#define COM_SCK_IN PC5
#define COM_SCK_OUT PC4


#define COM_INT_DDR DDRD
#define COM_INT_PORT PORTD
#define COM_INT_PIN PD2


#define CMD_PIN PINC
#define CMD_PORT PORTC
#define CMD_DDR DDRC

#define CMD_PIN_0 PC6
#define CMD_PIN_1 PC7

#define BUS_OUTPUT 0xFF
#define BUS_INPUT 0x00


#define COM_ADR 0b01000000

#define CMD_MA_IDLE 0x00
#define CMD_MA_OUT 0x01
#define CMD_MA_REQ 0x02
#define CMD_END 0x03

extern volatile unsigned char cDataReq;

void com_init();
void com_receive_chunk( uint8_t* uiLen );
void com_send_chunk(uint8_t uiLen);

void com_connect();
void com_disconnect();

void com_config(char cDirection);
char com_check();

void com_set_command(unsigned char cCommand);

#endif /* COM_H_ */
