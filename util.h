/*
 * util.h
 *
 *  Created on: 23.01.2011
 *      Author: gabm
 */

#ifndef UTIL_H_
#define UTIL_H_

#define sbi(port,bit) (port)|=(1<<(bit ))
#define cbi(port,bit) (port)&=~(1<<(bit))

#endif /* UTIL_H_ */
