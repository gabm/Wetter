#include "sensors.h"

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
	loop_until_bit_is_set(RANGE_PIN, RANGE_ECHO);

	//start timer with 1/8 prescaler
	sbi(TCCR1B, CS11);
	loop_until_bit_is_clear(RANGE_PIN, RANGE_ECHO);
	cbi(TCCR1B, CS11);

	//the factor 1.3 is from 1/6.441MHz*8
	//58 comes from 58cm per µs
	return TCNT1*1.3/58.0;
}


/**
 * This inits the ADC. We also do one dummy-readout.
 */
void adc_init(void)
{
	uint16_t result;

	ADMUX = (0 << REFS1) | (1 << REFS0); // interne Referenzspannung nutzen
	ADCSRA = (1 << ADPS1) | (1 << ADPS0); // Frequenzvorteiler
	ADCSRA |= (1 << ADEN); // ADC aktivieren


	ADCSRA |= (1 << ADSC); // eine ADC-Wandlung
	while (ADCSRA & (1 << ADSC))
		; // auf Abschluss der Konvertierung warten
	//dummy readout
	result = ADCW;
}
/**
 * One read-out for a certain channel (~a certain Pin from Port A) is performed.
 * @param channel which channel should be read? -> 0..7
 * @return 16-bit read-out result.
 */
uint16_t adc_read(uint8_t channel)
{
	// Kanal waehlen, ohne andere Bits zu beeinflu?en
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC); // eine Wandlung "single conversion"
	while (ADCSRA & (1 << ADSC))
		; // auf Abschluss der Konvertierung warten;
	return ADCW; // ADC auslesen und zur?ckgeben
}

/**
 * This writes one 8-bit Command to a DS1620.
 * @param cCommand the 8-bit command to be written.
 */
static void tmp_write(char cCommand)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		cbi(PORTB,SCK); //clock low
		if (cCommand & 0x01)
		{
			sbi(PORTB,MISO);
		}
		else
		{
			cbi(PORTB,MISO);
		}
		_delay_us(10);
		sbi(PORTB,SCK); //clock high
		_delay_us(30);
		cCommand = (cCommand >> 1);
	}
}

/**
 * One Temperature read-out is performed.
 * You _should not_ use this! Use tmp_getTemperature(char cTmpID) instead!
 * @return 16-bit read-out result.
 */
static uint16_t tmp_read()
{
	uint16_t result = 0x0000;
	uint16_t mask = 0x01;
	uint8_t uiTemp;
	for (uint8_t i = 0; i < 9; i++)
	{
		cbi(PORTB,SCK); //clock low
		_delay_us(5);
		uiTemp = PINB & 0b01000000;
		_delay_us(5);

		if (uiTemp)
		{
			//bit is one
			result |= mask;
		} //else bit is zero, do nothing
		sbi(PORTB,SCK);
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
void tmp_config(char cTmpID)
{
	//configure ports
	sbi(DDRB,SCK);
	sbi(DDRB,MISO);

	cbi(PORTB,SCK);
	cbi(PORTB,MISO);

	//connect to DS1620
	adr_set(cTmpID, 3);
	_delay_us(10);

	//write data
	tmp_write(0x0C);
	tmp_write(0x0A);

	//disconnect
	adr_set(0x00, 3);
	_delay_ms(30);
}

/**
 * Inits the DS1620. Has to be called once on system startup.
 * @param cTmpID which ID should be initialized?
 */
void tmp_init(char cTmpID)
{
	//configure ports
	sbi(DDRB,SCK);
	sbi(DDRB,MISO);

	cbi(PORTB,SCK);
	cbi(PORTB,MISO);

	//connect to DS1620
	adr_set(cTmpID, 3);
	_delay_us(10);

	//write command
	tmp_write(0xEE);
	_delay_us(10);

	//disconnect
	adr_set(0x00, 3);
}

/**
 * Get Temperature value from DS1620.
 * @param cTmpID which ID should be read?
 * @return 16-bit Value for current Temperature in 0.5 degree steps.
 */
uint16_t tmp_get(char cTmpID)
{
	uint16_t uiResult;
	//configure ports
	sbi(DDRB,SCK);
	sbi(DDRB,MISO);

	cbi(PORTB,SCK);
	cbi(PORTB,MISO);

	//connect to DS1620
	adr_set(cTmpID, 3);
	_delay_us(10);

	//write getTemp Command
	tmp_write(0xAA);

	//set MISO as Input
	cbi(DDRB,MISO);
	sbi(PORTB,MISO);
	_delay_us(10);

	uiResult = tmp_read();

	//disconnect
	adr_set(0x00, 3);
	_delay_ms(1);
	return uiResult;
}
