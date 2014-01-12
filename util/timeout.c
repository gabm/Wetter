/*
 * timeout.c
 *
 *  Created on: 05.12.2013
 *      Author: gabm
 */

#include "timeout.h"

void timeout_init(void) {   /* call at startup, let it run until needed */
   TCCR1A = 0;      /* "Normal" mode, */
   TCCR1B = (1 << CS12) | (0 << CS11) | (1 << CS10); /* prescale /1024 */
   return;
}
