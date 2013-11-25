/*
 * commands.h
 *
 *  Created on: 11.04.2012
 *      Author: gabm
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "../core/buffers.h"
#include "../sensors/sensors.h"

#define MAX_DATA_LENGTH 10
typedef struct {
	uint8_t uiCommand;
	unsigned char pData[MAX_DATA_LENGTH];
} SPacket;


//temperature
typedef uint16_t Temperature_Data_t;
#define CMD_TEMPERATURE_1 1
#define CMD_TEMPERATURE_2 2
#define CMD_TEMPERATURE_3 3
#define CMD_TEMPERATURE_4 4

//snow depth
typedef uint16_t Snow_Depth_Data;
#define CMD_SNOW_DEPTH 5

//pressure
typedef uint16_t Pressure_Data_t;
#define CMD_PRESSURE 6

//Commands
#define CMD_RESET 255

uint8_t cmd_parse();

#endif /* COMMANDS_H_ */
