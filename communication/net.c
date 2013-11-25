/*
 * net.c
 *
 *  Created on: 22.03.2012
 *      Author: gabm
 */

#include "net.h"

void net_receive()
{
	if (!com_check())
		return;

	com_connect();
	com_set_command(CMD_MA_REQ);
	com_config(BUS_INPUT);

	uint8_t uiLen =0;
	com_receive_chunk(&uiLen);
	com_disconnect();
	cDataReq = 0;
	sbi(GICR,INT0);

	uiLen = cmd_parse();
	net_send(uiLen);
}
void net_send(uint8_t uiLen)
{
	com_connect();
	com_set_command(CMD_MA_OUT);
	com_config(BUS_OUTPUT);
	com_send_chunk(uiLen);
	com_disconnect();

}
