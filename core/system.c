#include "system.h"

/**
 * Inits the Latches to all-Zero values.
 * This should be done once.
 */
void sys_adr_init(void)
{
	//Configure output
	sbi(DDRD,PD4);
	sbi(DDRD,PD5);
	sbi(DDRD,PD7);

	sbi(DDRB,PB4);
	sbi(PORTB,PB4);

	sys_adr_set(0x00, 1);
	sys_adr_set(0x00, 2);
	sys_adr_set(0x00, 3);
	sys_adr_set(0x00, 4);
}
/**
 * Reset all latches to zero (low).
 */
void sys_adr_reset()
{
	sys_adr_set(0x00, 1);
	sys_adr_set(0x00, 2);
	sys_adr_set(0x00, 3);
	sys_adr_set(0x00, 4);
}

/**
 * sets address to a specific latch
 * @param cAddress the address to set
 * @param uiLatchNr	the latch number
 */
void sys_adr_set(char cAddress, uint8_t uiLatchNr)
{
	bus_output();

	//select wanted latch - 3 bits input
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD4);
	}
	else
	{
		cbi(PORTD,PD4);
	}

	uiLatchNr = (uiLatchNr >> 1);
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD5);
	}
	else
	{
		cbi(PORTD,PD5);
	}

	uiLatchNr = (uiLatchNr >> 1);
	if (uiLatchNr & 0x01)
	{
		sbi(PORTD,PD7);
	}
	else
	{
		cbi(PORTD,PD7);
	}

	//set address to databus
	PORTC = cAddress;

	//select Latch
	//enable 3-8 decoder
	cbi(PORTB,PB4);
	_delay_us(10);
	//to clean up, disable 3-8 decoder
	sbi(PORTB,PB4);
}
