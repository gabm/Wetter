/*
 * range.c
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */
#include "range.h"

void range_init(void)
{
	//trigger pin as output and low level
	cbi(RANGE_PORT,RANGE_TRIG);
	sbi(RANGE_DDR,RANGE_TRIG);

	//echo pin as input with pullup
	sbi(RANGE_PORT,RANGE_ECHO);
	cbi(RANGE_DDR,RANGE_ECHO);
}
uint16_t range_get(void)
{

	//send out one pulse
	sbi(RANGE_PORT, RANGE_TRIG);
	_delay_us(15);
	cbi(RANGE_PORT,RANGE_TRIG);

	//wait for result
	TCNT1 = 0x0000;
	//wait for high val
	//while(!(timeout_event() || bit_is_set(RANGE_PIN, RANGE_ECHO)));
	loop_until_bit_is_set(RANGE_PIN, RANGE_ECHO);
	//timeout_stop();

	//if (timeout_event())
		//return 0xFFFF;


	//start timer with 1/8 prescaler
	sbi(TCCR1B, CS11);
	//timeout_start_ms(500);
	//while(!(timeout_event() || bit_is_clear(RANGE_PIN, RANGE_ECHO)));
	loop_until_bit_is_clear(RANGE_PIN, RANGE_ECHO);
	cbi(TCCR1B, CS11);
	//timeout_stop();

	//if (timeout_event())
		//return 0xFFFF;
	//the factor 1.3 is from 1/6.441MHz*8
	//58 comes from 58cm per µs
	return TCNT1*1.3/58.0;
}
