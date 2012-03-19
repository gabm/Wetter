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

#endif /* SENSORS_H_ */
