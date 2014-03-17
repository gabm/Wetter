/*
 * packets.h
 *
 *  Created on: 05.12.2013
 *      Author: gabm
 */

#ifndef PACKETS_H_
#define PACKETS_H_

#include "../core/buffers.h"

///////////////////////////////////////////////////////////////////
//////////// General Packet
#define MAX_PACKET_LENGTH (MAX_CHUNK_SIZE - 1)
typedef struct {
	uint8_t uiCommandID;
	unsigned char pData[MAX_PACKET_LENGTH - 1];
} SPacket;

///////////////////////////////////////////////////////////////////
//////////// Command IDs
#define CMD_SENSOR_READOUT 1
#define CMD_RESET 255




///////////////////////////////////////////////////////////////////
//////////// Sensor Readout Packet
#define MAX_SENSOR_READOUT_PACKET_LENGTH (MAX_PACKET_LENGTH-1)

typedef struct {
	uint8_t uiSensorID;
	unsigned char pData[MAX_SENSOR_READOUT_PACKET_LENGTH - 1];
} SSensorReadoutPacket;

///////////////////////////////////////////////////////////////////
//////////// Sensor IDs
#define SENSOR_ID_TEMPERATURE_1 1
#define SENSOR_ID_TEMPERATURE_2 2
#define SENSOR_ID_TEMPERATURE_3 3
#define SENSOR_ID_TEMPERATURE_4 4
#define SENSOR_ID_SNOW_DEPTH 5

#endif /* PACKETS_H_ */
