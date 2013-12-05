/*
 * temperature.c
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#include "temperature.h"

/**
 * This writes one 8-bit Command to a DS1620.
 * @param cCommand the 8-bit command to be written.
 */
void temp_write(char cCommand)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		cbi(PORTB,SYS_SCK); //clock low
		if (cCommand & 0x01)
		{
			sbi(PORTB,SYS_MISO);
		}
		else
		{
			cbi(PORTB,SYS_MISO);
		}
		_delay_us(10);
		sbi(PORTB,SYS_SCK); //clock high
		_delay_us(30);
		cCommand = (cCommand >> 1);
	}
}

/**
 * One Temperature read-out is performed.
 * You _should not_ use this! Use tmp_getTemperature(char cTmpID) instead!
 * @return 16-bit read-out result.
 */
uint16_t temp_read()
{
	uint16_t result = 0x0000;
	uint16_t mask = 0x01;
	uint8_t uiTemp;
	for (uint8_t i = 0; i < 9; i++)
	{
		cbi(PORTB,SYS_SCK); //clock low
		_delay_us(5);
		uiTemp = PINB & 0b01000000;
		_delay_us(5);

		if (uiTemp)
		{
			//bit is one
			result |= mask;
		} //else bit is zero, do nothing
		sbi(PORTB,SYS_SCK);
		_delay_us(10);
		mask = (mask << 1);
	}
	return result;
}
/**
 * Config the DS1620 to perform continious conversion in PC-Mode.
 * Attention: _DO NOT_ call this everytime. The Information is written to EEPROM
 * which only allows 10.000 write-cycles. A DS1620 has to be configured only once!
 * @param cTmpID which ID should be configured?
 */
void temp_config(char cTmpID)
{
	//configure ports
	sbi(DDRB,SYS_SCK);
	sbi(DDRB,SYS_MISO);

	cbi(PORTB,SYS_SCK);
	cbi(PORTB,SYS_MISO);

	//connect to DS1620
	sys_adr_set(cTmpID, 3);
	_delay_us(10);

	//write data
	temp_write(0x0C);
	temp_write(0x0A);

	//disconnect
	sys_adr_set(0x00, 3);
	_delay_ms(30);
}

/**
 * Inits the DS1620. Has to be called once on system startup.
 * @param cTmpID which ID should be initialized?
 */
void temp_init(char cTmpID)
{
	//configure ports
	sbi(DDRB,SYS_SCK);
	sbi(DDRB,SYS_MISO);

	cbi(PORTB,SYS_SCK);
	cbi(PORTB,SYS_MISO);

	//connect to DS1620
	sys_adr_set(cTmpID, 3);
	_delay_us(10);

	//write command
	temp_write(0xEE);
	_delay_us(10);

	//disconnect
	sys_adr_set(0x00, 3);
}

/**
 * Get Temperature value from DS1620.
 * @param cTmpID which ID should be read?
 * @return 16-bit Value for current Temperature in 0.5 degree steps.
 */
uint16_t temp_get(char cTmpID)
{
	uint16_t uiResult;
	//configure ports
	sbi(DDRB,SYS_SCK);
	sbi(DDRB,SYS_MISO);

	cbi(PORTB,SYS_SCK);
	cbi(PORTB,SYS_MISO);

	//connect to DS1620
	sys_adr_set(cTmpID, 3);
	_delay_us(10);

	//write getTemp Command
	temp_write(0xAA);

	//set MISO as Input
	cbi(DDRB,SYS_MISO);
	sbi(PORTB,SYS_MISO);
	_delay_us(10);

	uiResult = temp_read();

	//disconnect
	sys_adr_set(0x00, 3);
	_delay_ms(1);
	return uiResult;
}
