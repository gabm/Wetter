/*
 * time.h
 *
 *  Created on: 19.01.2011
 *      Author: gabm
 */

#ifndef TIME_H_
#define TIME_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#include "util.h"
#include "data.h"

typedef struct
{
	uint8_t uiMinuteParityError;
	uint8_t uiHourParityError;
	uint8_t uiDateParityError;
} SDCF77Parity;



void time_init(void);

#endif /* TIME_H_ */
