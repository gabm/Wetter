#include "main.h"
#include "util.h"
#include "system.h"
#include "uart.h"
#include "sensors.h"
#include "time.h"
#include "com.h"
#include "data.h"
#include "net.h"

int main(void)
{
	//init
	adr_init();
	adc_init();
	uart_init();
	com_init();

	//sensors
	tmp_init(0x01);
	tmp_init(0x02);


	sei();

	uart_send("ready");
	_delay_ms(2000);


	//uint16_t count =0;
	while(1)
	{
		//count++;
		net_receive();
		/*if (count == 0x00ff)
		{
			count =0;
			uint16_t uiResult = adc_read(0x00);
			uart_send_char(uiResult >> 8);
			uart_send_char(uiResult);
		}*/
	}

	return 0;
}
