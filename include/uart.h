/*
 * uart.h
 *
 *  Created on: 23.01.2011
 *      Author: gabm
 */

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "string.h"
#include "system.h"
#include "util.h"

#define BAUD 9600UL      // Baudrate
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
#error Systematischer Fehler der Baudrate gr?sser 1% und damit zu hoch!
#endif

//functions
void uart_send(char* sToSend);
void uart_send_char(unsigned char sToSend);
void uart_init(void);

#endif /* UART_H_ */
