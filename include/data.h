/*
 * data.h
 *
 *  Created on: 20.03.2012
 *      Author: gabm
 */

#ifndef DATA_H_
#define DATA_H_

#include <avr/io.h>

//*****************************************
//time
#define DATA_DATETIME 0
typedef struct
{
	uint8_t	uiSecond;
	uint8_t uiMinute;
	uint8_t uiHour;
	uint8_t uiDay;
	uint8_t uiMonth;
	uint8_t uiYear;
} SDateTime;

extern volatile SDateTime oDateTime;

//*****************************************
//temperature sensor
#define DATA_TMP 1
typedef struct
{
	SDateTime oTimeStamp;
	uint16_t uiTemperature;
} STempData;
extern STempData oCurrentTempData;

//*****************************************
//range detector
#define DATA_RANGE 2
typedef struct
{
	SDateTime oTimeStamp;
	uint8_t uiRange;
} SRangeData;
extern SRangeData oCurrentRangeData;

//*****************************************
//Data Declaration
extern unsigned char* pData[];
extern uint8_t pDataSizes[];

#endif /* DATA_H_ */
