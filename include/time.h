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

//time
typedef struct
{
	uint8_t	uiSecond;
	uint8_t uiMinute;
	uint8_t uiHour;
	uint8_t uiDay;
	uint8_t uiMonth;
	uint8_t uiYear;
} SDateTime;

typedef struct
{
	uint8_t uiMinuteParityError;
	uint8_t uiHourParityError;
	uint8_t uiDateParityError;
} SDCF77Parity;

extern volatile SDateTime oDateTime;
void time_init(void);

#endif /* TIME_H_ */
