/*
 * com_neu.h
 *
 *  Created on: 19.03.2012
 *      Author: gabm
 */

#ifndef COM_H_
#define COM_H_

#include <avr/io.h>
#include "system.h"
#include "util.h"

#define OUT_SIG_BYTE PC4
#define OUT_SIG_CHUNK PC5
#define IN_SIG_BYTE PC6
#define IN_SIG_CHUNK PC7

#define BUS_DDR DDRC
#define BUS_PORT PORTC
#define BUS_PIN PINC

#define OUT_SIG_DDR DDRC
#define OUT_SIG_PORT PORTC
#define OUT_SIG_PIN PINC

#define IN_SIG_DDR DDRC
#define IN_SIG_PORT PORTC
#define IN_SIG_PIN PINC


#define STATUS_NORMAL 0
#define STATUS_LAST_BYTE 1

#define BUS_OUTPUT 0b00111111
#define BUS_INPUT 0b00110000

#define MAX_CHUNK_SIZE 10

#define NET_ADR 0b01000000

unsigned char com_receive_buffer[MAX_CHUNK_SIZE];
unsigned char com_send_buffer[MAX_CHUNK_SIZE];

void com_init();
void com_send(uint8_t uiLen);
void com_receive(uint8_t* uiLen);

#endif /* COM_H_ */
