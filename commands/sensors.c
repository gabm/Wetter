/*
 * sensors.c
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */



#include "sensors.h"


//this time includes are in the c-file... this is intended!
#include "../sensors/temperature.h"
#include "../sensors/range.h"



uint8_t sensors_readout(SSensorReadoutPacket* pSensorRequest, SSensorReadoutPacket* pSensorResponse )
{
	uint8_t uiPacketLength =0;
	switch (pSensorRequest->uiSensorID) {
		case SENSOR_ID_TEMPERATURE_1:
		{
			Temperature_Data_t* pTemp = (Temperature_Data_t*)pSensorResponse->pData;
			*pTemp = temp_get(0b00000001);
			uiPacketLength += sizeof(Temperature_Data_t);
			break;
		}
		case SENSOR_ID_TEMPERATURE_2:
		{
			Temperature_Data_t* pTemp = (Temperature_Data_t*)pSensorResponse->pData;
			*pTemp = temp_get(0b00000010);
			uiPacketLength += sizeof(Temperature_Data_t);
			break;
		}
		case SENSOR_ID_TEMPERATURE_3:
		{
			Temperature_Data_t* pTemp = (Temperature_Data_t*)pSensorResponse->pData;
			*pTemp = temp_get(0b00000100);
			uiPacketLength += sizeof(Temperature_Data_t);
			break;
		}
		case SENSOR_ID_TEMPERATURE_4:
		{
			Temperature_Data_t* pTemp = (Temperature_Data_t*)pSensorResponse->pData;
			*pTemp = temp_get(0b00001000);
			uiPacketLength += sizeof(Temperature_Data_t);
			break;
		}
		case SENSOR_ID_SNOW_DEPTH:
		{
			Snow_Depth_Data_t* pSnowDepth = (Snow_Depth_Data_t*)pSensorResponse->pData;
			*pSnowDepth = range_get();
			uiPacketLength += sizeof(Snow_Depth_Data_t);
			break;
		}
		default:
			break;
	}
	pSensorResponse->uiSensorID = pSensorRequest->uiSensorID;
	uiPacketLength++;
	return uiPacketLength;
}


