#include "uart.h"

/**
 * Perform a 8-bit BCD to 8-bit Binary conversion.
 * @param uiIn 8-bit BCD in
 * @return 8-bit Binary
 */
uint8_t BCD_to_Bin(uint8_t uiIn)
{
	return ((((uiIn >> 4) * 5) << 1) + (uiIn & 0x0F));
}
