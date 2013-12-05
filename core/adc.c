/*
 * adc.c
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#include "adc.h"

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
