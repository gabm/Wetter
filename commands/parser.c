/*
 * parser.c
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#include "parser.h"


uint8_t parser_parse()
{
	SPacket* pRequest = (SPacket*)com_receive_buffer;
	SPacket* pResponse = (SPacket*)com_send_buffer;

	uint8_t uiLength =0;
	switch (pRequest->uiCommandID)
	{
		case CMD_SENSOR_READOUT:
		{
			uiLength += sensors_readout((SSensorReadoutPacket*)pRequest->pData, (SSensorReadoutPacket*)pResponse->pData);
			break;
		}
	}

	pResponse->uiCommandID = pRequest->uiCommandID;
	uiLength +=1;
	return uiLength;
}
