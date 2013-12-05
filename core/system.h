/*
 * system.h
 *
 *  Created on: 23.01.2011
 *      Author: gabm
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../util.h"


//8-bit parallel bus
#define bus_input() DDRC = 0x00;
#define bus_output() DDRC = 0xFF;

//SPI-Bus definitions
#define SYS_SCK PB7
#define SYS_MOSI PB5
#define SYS_MISO PB6

//addressing
void sys_adr_init(void);
void sys_adr_set(char cAddress, uint8_t uiLatchNr);
void sys_adr_reset();

#endif /* SYSTEM_H_ */
