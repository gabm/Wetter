/*
 * timeout.h
 *
 *  Created on: 05.12.2013
 *      Author: gabm
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../util.h"

void timeout_start(uint8_t uiTimeout);
void timeout_stop();

#define   TICKS_PER_MSEC (F_CPU/1024/255/1000)   /* ticks/sec with prescale /1024 */

extern volatile uint8_t uiOverflow;

#define timeout_start_ms(time_ms) timeout_start(time_ms*TICKS_PER_MSEC)
#define timeout_event() uiOverflow == 1

#endif /* TIMEOUT_H_ */
