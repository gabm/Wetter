/*
 * sensors.h
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#ifndef SENSORS_H_
#define SENSORS_H_

#include <avr/io.h>
#include "packets.h"

//temperature
typedef uint16_t Temperature_Data_t;

//snow depth
typedef uint16_t Snow_Depth_Data_t;
#define SENSOR_ID_SNOW_DEPTH 5


uint8_t sensors_readout(SSensorReadoutPacket* pSensorRequest, SSensorReadoutPacket* pSensorResponse );

#endif /* SENSORS_H_ */
