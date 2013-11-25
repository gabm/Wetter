/*
 * net.h
 *
 *  Created on: 22.03.2012
 *      Author: gabm
 */

#ifndef NET_H_
#define NET_H_
#include "com.h"
#include "../core/buffers.h"
#include "../commands/commands.h"

void net_send(uint8_t uiLen);
void net_receive();
#endif /* NET_H_ */
