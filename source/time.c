/*
 * time.c
 *
 *  Created on: 19.01.2011
 *      Author: gabm
 */
#include "time.h"

/**
 * a struct to hold parity error-counts
 */

//globals
volatile SDateTime oDateTime;

//file-globals
static volatile SDateTime oTempDateTime;
static volatile SDCF77Parity oDCF77Parity;
static volatile uint8_t uiBitCount;
static volatile uint8_t uiTimer2Count;

/////////////////////////////////////////////////////////////////////////////
//////			Time related
/////////////////////////////////////////////////////////////////////////////
/**
 * This resets the official time to zero-values.
 * Only use this in the beginning!
 */
static void time_reset(void)
{
	oDateTime.uiSecond = 0x00;
	oDateTime.uiMinute = 0x00;
	oDateTime.uiHour = 0x00;
	oDateTime.uiDay = 0x00;
	oDateTime.uiMonth = 0x00;
	oDateTime.uiYear = 0x00;
}

/**
 * reset the temporary time object
 */
static void time_tmp_reset(void)
{

	oTempDateTime.uiMinute = 0x00;
	oTempDateTime.uiHour = 0x00;
	oTempDateTime.uiDay = 0x00;
	oTempDateTime.uiMonth = 0x00;
	oTempDateTime.uiYear = 0x00;

	oDCF77Parity.uiMinuteParityError = 0x00;
	oDCF77Parity.uiHourParityError = 0x00;
	oDCF77Parity.uiDateParityError = 0x00;

	uiBitCount = 0x00;
	uiTimer2Count = 0x00;
}

/**
 * Inits the time measurement functions on the system-side.
 * Enables global-interrupts, uses timer 1 and enables external interrupt 0.
 */
void time_init(void)
{
	//init
	time_tmp_reset();
	time_reset();

	//setup timer 2 for dcf77 measurement
	cbi(TCCR2,WGM21);
	cbi(TCCR2,WGM20);

	//set timer 2 register to 0
	TCNT2 = 0;

	//set prescaler for timer 2 to 256
	sbi(TCCR2,CS22);
	sbi(TCCR2,CS21);
	cbi(TCCR2,CS20);

	//enable timer 2 overflow
	sbi(TIMSK,TOIE2);


	//external interrupt on any logical change
	sbi(MCUCR,ISC10);

	//set PD3 as input
	cbi(DDRD,PD3);

	//enable external interrupt 1
	sbi(GICR,INT1);

	//preset Timer 1 to 41536
	TCNT1 = 41536;

	//enable timer 1 overflow interrupt
	sbi(TIMSK,TOIE1);

	//set timer 1 prescaler
	sbi(TCCR1B,CS12);

	//enable gloabal interrupts
	sei();
}

/////////////////////////////////////////////////////////////////////////////
//////			DCF77
/////////////////////////////////////////////////////////////////////////////

/**
 * This function parses the time, after it was received. It has does some error handling
 * and increments the time manually, if necessary.
 * _DO NOT_ call this manually!!!
 */
static void time_parse(void)
{
	//error check
	if (uiBitCount != 59) return;
	if (oDCF77Parity.uiMinuteParityError != 0x00) return;
	if (oDCF77Parity.uiHourParityError != 0x00) return;
	if (oDCF77Parity.uiDateParityError != 0x00) return;

	//if we get here, no error has occurred
	//we can write our date to minute
	oDateTime.uiMinute = BCD_to_Bin(oTempDateTime.uiMinute >> 1);
	oDateTime.uiHour = BCD_to_Bin(oTempDateTime.uiHour >> 2);
	oDateTime.uiDay = BCD_to_Bin(oTempDateTime.uiDay >> 2);
	oDateTime.uiMonth = BCD_to_Bin(oTempDateTime.uiMonth >> 3);
	oDateTime.uiYear = BCD_to_Bin(oTempDateTime.uiYear);

	oDateTime.uiSecond = 0x00;
}

/**
 * Interrupt Service Routine for external Interrupt 1. Here we fill in the temp date.
 */
ISR(INT1_vect)
{
	//uart_send("int");
	//uart_send_char(uiTimer2Count);
	uint8_t uiLocalTimer = uiTimer2Count;
	uiTimer2Count =0;
	if (!(PIND & 0b00001000))
	{
		//falling edge
		char cBit = 0x00;

		//Bit 1
		if (uiLocalTimer > 14)
			cBit = 0x80;
		if (uiBitCount > 20)
		{
			if (uiBitCount <= 27)
			{
				//Minute
				oTempDateTime.uiMinute = (oTempDateTime.uiMinute >> 1) + cBit;
				oDCF77Parity.uiMinuteParityError += cBit;
			}
			else if (uiBitCount == 28)
			{
				//Minute Parity
				oDCF77Parity.uiMinuteParityError += cBit;
			}
			else if (uiBitCount <= 34)
			{
				//Stunde
				oTempDateTime.uiHour = (oTempDateTime.uiHour >> 1) + cBit;
				oDCF77Parity.uiHourParityError += cBit;
			}
			else if (uiBitCount == 35)
			{
				//Stunde Parity
				oDCF77Parity.uiHourParityError += cBit;
			}
			else if (uiBitCount <= 41)
			{
				//Tag
				oTempDateTime.uiDay = (oTempDateTime.uiDay >> 1) + cBit;
				oDCF77Parity.uiDateParityError += cBit;
			}
			else if (uiBitCount <= 44)
			{
				//Wochentag
				oDCF77Parity.uiDateParityError += cBit;
			}
			else if (uiBitCount <= 49)
			{
				//Monat
				oTempDateTime.uiMonth = (oTempDateTime.uiMonth >> 1) + cBit;
				oDCF77Parity.uiDateParityError += cBit;
			}
			else if (uiBitCount <= 57)
			{
				//Jahr
				oTempDateTime.uiYear = (oTempDateTime.uiYear >> 1) + cBit;
				oDCF77Parity.uiDateParityError += cBit;
			}
			else if (uiBitCount == 58)
			{
				//Date Parity Bit
				oDCF77Parity.uiDateParityError += cBit;
			}
		}
		uiBitCount++;
	}
}

/**
 * ISR for timer2. This timer overflows every >10ms. These ticks are counted
 * and used for evaluation of dcf77 signal. If we reach >10ms*94 without any
 * interrupt from dcf77, we either have received a correct date or got an error.
 */
ISR(TIMER2_OVF_vect)
{
	//if there is more than one second elapsed
	//since last interrupt we have sync bit or error
	if (uiTimer2Count == 94)
	{
		time_parse();
		time_tmp_reset();
	}

	uiTimer2Count++;

}
/////////////////////////////////////////////////////////////////////////////
//////			Soft Clock
/////////////////////////////////////////////////////////////////////////////

/**
 * Checks, whether provided year is a Leap year or not
 * @param uiYear Which year? Format: YYYY (e.g. 2011)
 * @return 1 for true; 0 for false
 */
static uint8_t isLeapYear(uint16_t uiYear)
{
   if(uiYear%400==0) return 1;
   if(uiYear%100==0) return 0;
   if(uiYear%  4==0) return 1;
   return 0;
}

/**
 * How many days has the provided month.
 * @param uiMonth Month number; 1..12
 * @param uiYear Year number, Format: YYYY (e.g. 2011)
 * @return the number of days in this month
 */
static uint8_t getDaysPerMonth(uint8_t uiMonth,uint16_t uiYear)
{
   if(uiMonth == 2)
   {
      if(isLeapYear(uiYear) == 1)
         return 28;
      else
         return 29;
   }
   else if(uiMonth==4 || uiMonth==6 || uiMonth==9 || uiMonth==11)
      return 30;
   else
      return 31;
};

/**
 * Adds one second to the official DateTime object. Respects Leap Year rules!
 */
static void time_inc()
{
	oDateTime.uiSecond++;
	if (oDateTime.uiSecond == 60)
	{
		oDateTime.uiSecond = 0;
		oDateTime.uiMinute++;
		if (oDateTime.uiMinute == 60)
		{
			oDateTime.uiMinute = 0;
			oDateTime.uiHour++;
			if (oDateTime.uiHour == 24)
			{
				oDateTime.uiHour = 0;
				oDateTime.uiDay++;
				if (oDateTime.uiDay == getDaysPerMonth(oDateTime.uiMonth,oDateTime.uiYear+2000)+1)
				{
					oDateTime.uiMonth++;
					if (oDateTime.uiMonth == 13)
					{
						oDateTime.uiMonth = 1;
						oDateTime.uiYear++;
					}
				}
			}
		}
	}
}


/**
 * Interrupt Service Routine for Timer 1 Overflow!
 * The timer is reset for a timeout in 1 Second.
 */
ISR(TIMER1_OVF_vect)
{
	//reset timer
	TCNT1 = 41536;

	time_inc();

}
