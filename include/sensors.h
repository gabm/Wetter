/*
 * sensors.h
 *
 *  Created on: 23.01.2011
 *      Author: gabm
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <avr/io.h>
#include <util/delay.h>

#include "util.h"
#include "system.h"

//temperature sensor public interface
//tmp_config(char) -> handle with care!
//only 10000 write-cycles to EEPROM are allowed
void tmp_config(char cTmpID);

void tmp_init(char cTmpID);
uint16_t tmp_get(char cTmpID);

//analog-digital-converter
uint16_t adc_read(uint8_t channel);
void adc_init(void);


//range defines
#define RANGE_PORT PORTD
#define RANGE_PIN  PIND
#define RANGE_DDR  DDRD
#define RANGE_TRIG PD3
#define RANGE_ECHO PD6


//range sensor
void range_init(void);
uint16_t range_get(void);

#endif /* SENSORS_H_ */
