#include "main.h"
#include "util.h"
#include "system.h"
#include "uart.h"
#include "sensors.h"
#include "time.h"
#include "com.h"

void dbg_print_time()
{
	uart_send_char(oDateTime.uiSecond);
	uart_send_char(oDateTime.uiMinute);
	uart_send_char(oDateTime.uiHour);
	uart_send_char(oDateTime.uiDay);
	uart_send_char(oDateTime.uiMonth);
	uart_send_char(oDateTime.uiYear);
}
char cPassed;
void tmp_meas()
{
	uint16_t uiTemp = tmp_get(0x01);
	char cData [9] = {oDateTime.uiSecond,oDateTime.uiMinute,oDateTime.uiHour,oDateTime.uiDay,oDateTime.uiMonth, oDateTime.uiYear, 0x01, uiTemp>>8,uiTemp};
	mem_write_data(cData,9);

	uiTemp = tmp_get(0x02);
	char cData2 [9] = {oDateTime.uiSecond,oDateTime.uiMinute,oDateTime.uiHour,oDateTime.uiDay,oDateTime.uiMonth, oDateTime.uiYear, 0x02, uiTemp>>8,uiTemp};
	mem_write_data(cData2,9);
}

void range_meas()
{
	uint8_t uiRange = adc_read(0x00);
	char cData [8] = {oDateTime.uiSecond,oDateTime.uiMinute,oDateTime.uiHour,oDateTime.uiDay,oDateTime.uiMonth, oDateTime.uiYear, 0x03, uiRange};
	mem_write_data(cData,8);
}

void scheduler()
{
	if (oDateTime.uiYear == 0) return;
	if(oDateTime.uiMinute % 5 == 0 && oDateTime.uiSecond == 10)
	{
		if (cPassed == 0x01) return;
		cPassed = 0x01;
		dbg_print_time();
		tmp_meas();
		range_meas();
	} else {
		cPassed = 0x00;
	}
}

int main(void)
{
	//init
	adr_init();
	com_init();

	_delay_ms(2000);
	while(1)
	{
		_delay_ms(1000);
		unsigned char* itr = com_send_buffer;
		*itr++ = 0x01;
		*itr++ = 0x02;
		*itr++ = 0x03;
		*itr++ = 0x04;
		com_send(4);
	}

	return 0;
}
