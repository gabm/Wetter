/*
 * data.c
 *
 *  Created on: 20.03.2012
 *      Author: gabm
 */


#include "data.h"

volatile SDateTime oDateTime;
STempData oCurrentTempData;
SRangeData oCurrentRangeData;


unsigned char* pData[] = {
		(unsigned char*)&oDateTime,
		(unsigned char*)&oCurrentTempData,
		(unsigned char*)&oCurrentRangeData
};

uint8_t pDataSizes[] = {
		sizeof(SDateTime),
		sizeof(STempData),
		sizeof(SRangeData)
};
