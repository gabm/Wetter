#include "main.h"
#include "core/uart.h"

int main(void)
{
	//init
	adr_init();
	uart_init();
	com_init();

	//sensors
	tmp_init(0x01);
	tmp_init(0x02);
	range_init();

	sei();


	_delay_ms(2000);
	uart_send("ready");

	while(1)
	{
		net_receive();
	}

	return 0;
}
