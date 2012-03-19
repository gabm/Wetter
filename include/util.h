/*
 * util.h
 *
 *  Created on: 23.01.2011
 *      Author: gabm
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include "uart.h"

#define sbi(port,bit) (port)|=(1<<(bit ))
#define cbi(port,bit) (port)&=~(1<<(bit))

uint8_t BCD_to_Bin(uint8_t uiIn);

#endif /* UTIL_H_ */
