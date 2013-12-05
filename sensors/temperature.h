/*
 * temperature.h
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#ifndef TEMPERATURE_H_
#define TEMPERATURE_H_

#include "../core/system.h"

//temperature sensor public interface
//tmp_config(char) -> handle with care!
//only 10000 write-cycles to EEPROM are allowed
void temp_config(char cTmpID);

void temp_init(char cTmpID);
uint16_t temp_get(char cTmpID);

#endif /* TEMPERATURE_H_ */
