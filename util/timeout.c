/*
 * timeout.c
 *
 *  Created on: 05.12.2013
 *      Author: gabm
 */

#include "timeout.h"
volatile uint8_t uiTimeoutCounter =0;
volatile uint8_t uiTimeoutTarget =0;
volatile uint8_t uiOverflow =0;

void timeout_start(uint8_t uiTimeout) {

	//reset counter
	TCNT2 = 0x00;
	uiTimeoutCounter =0;
	uiTimeoutTarget = uiTimeout;
	uiOverflow =0;

	//enable overflow interrupt
	sbi(TIMSK, TOIE2);
	sei();

	//start timer
	TCCR2 = (1 << CS12) | (1 << CS11) | (1 << CS10); /* prescale /1024 */

   return;
}

void timeout_stop()
{
	TCCR2 = (0 << CS12) | (0 << CS11) | (0 << CS10);
}

ISR(TIMER2_OVF_vect)
{
	if (uiTimeoutCounter++ == uiTimeoutTarget)
		uiOverflow = 1;

}
