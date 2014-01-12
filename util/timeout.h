/*
 * timeout.h
 *
 *  Created on: 05.12.2013
 *      Author: gabm
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <avr/io.h>

void timeout_init();


#define   TICKS_PER_SEC (F_CPU/1024)   /* ticks/sec with prescale /1024 */
#define   TIMEOUT_TIME (1 * TICKS_PER_SEC) /* timeout: 1 second */
#define   timeout_reset() do { TCNT1 = 0; } while (0)
#define   timeout_event() (TCNT1 >= TIMEOUT_TIME)


extern uint16_t uiCurrentTimeout;
#define   timeout_set(TIME_USEC) uiCurrentTimeout = TIME_USEC;


#endif /* TIMEOUT_H_ */
