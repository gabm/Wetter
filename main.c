#include "main.h"

#include "core/uart.h"
#include "communication/com.h"
#include "communication/net.h"
#include "sensors/temperature.h"
#include "sensors/range.h"
#include "util/timeout.h"

int main(void)
{
	// system init
	sys_adr_init();
	uart_init();
	com_init();

	//sensors
	temp_init(0x01);
	temp_init(0x02);
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
