/*
 * range.h
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#ifndef RANGE_H_
#define RANGE_H_

#include <avr/io.h>
#include <util/delay.h>

#include "../util.h"
#include "../util/timeout.h"

//range defines
#define RANGE_PORT PORTD
#define RANGE_PIN  PIND
#define RANGE_DDR  DDRD
#define RANGE_TRIG PD3
#define RANGE_ECHO PD6

//range sensor
void range_init(void);
uint16_t range_get(void);

#endif /* RANGE_H_ */
