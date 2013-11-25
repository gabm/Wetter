/*
 * commands.c
 *
 *  Created on: 11.04.2012
 *      Author: gabm
 */
#include "commands.h"

uint8_t cmd_parse()
{
	SPacket* pInPacket = (SPacket*)com_receive_buffer;
	SPacket* pOutPacket = (SPacket*)com_send_buffer;

	uint8_t uiLength =0;
	switch (pInPacket->uiCommand)
	{
	case CMD_TEMPERATURE_1:
	{
		Temperature_Data_t* TempData = (Temperature_Data_t*)pOutPacket->pData;
		*TempData = tmp_get(0x01);
		uiLength += sizeof(Temperature_Data_t);
		break;
	}
	case CMD_TEMPERATURE_2:
	{
		Temperature_Data_t* TempData = (Temperature_Data_t*)pOutPacket->pData;
		*TempData = tmp_get(0x02);
		uiLength += sizeof(Temperature_Data_t);
		break;
	}

	case CMD_SNOW_DEPTH:
	{
		Snow_Depth_Data* SnowDepthData = (Snow_Depth_Data*)pOutPacket->pData;
		*SnowDepthData = range_get();
		uiLength += sizeof(Snow_Depth_Data);
		break;
	}
	case CMD_PRESSURE:
	{
		Pressure_Data_t* PressureData = (Pressure_Data_t*)pOutPacket->pData;
		*PressureData = adc_read(0);
		uiLength += sizeof(Pressure_Data_t);
		break;
	}
	}

	pOutPacket->uiCommand = pInPacket->uiCommand;
	uiLength +=1;
	return uiLength;

}
