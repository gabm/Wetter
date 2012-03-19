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

#include "util.h"
#include "uart.h"


//8-bit parallel bus
#define bus_input() DDRC = 0x00;
#define bus_output() DDRC = 0xFF;

//SPI-Bus definitions
#define SCK PB7
#define MOSI PB5
#define MISO PB6

//addressing
void adr_init(void);
void adr_set(char cAddress, uint8_t uiLatchNr);
void adr_reset();

//memory definitions
#define WR PB3
#define RD PB2

//memory access
void mem_init(void);
void mem_chip_erase(void);
void mem_write_data(char* pData, uint8_t size);

void mem_print_sector(uint8_t uiSector);
void mem_sector_erase(uint8_t uiSector);

#endif /* SYSTEM_H_ */
