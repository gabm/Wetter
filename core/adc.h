/*
 * adc.h
 *
 *  Created on: 25.11.2013
 *      Author: gabm
 */

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
//analog-digital-converter
uint16_t adc_read(uint8_t channel);
void adc_init(void);


#endif /* ADC_H_ */
