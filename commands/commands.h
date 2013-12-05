/*
 * commands.h
 *
 *  Created on: 11.04.2012
 *      Author: gabm
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <avr/io.h>
#include "../core/buffers.h"


#define MAX_PACKET_LENGTH 10
typedef struct {
	uint8_t uiCommand;
	unsigned char pData[MAX_PACKET_LENGTH];
} SPacket;


//Commands
#define CMD_SENSOR_READOUT 1
#define CMD_RESET 255



#endif /* COMMANDS_H_ */
