/*
 * math.c
 *
 * Created: 12/18/2023 1:18:56 AM
 *  Author: Mahmoud Sofar
 */ 
#include "math.h"

/*under devloping */
uint32_t power(uint8_t base , uint8_t exponent)
{
	uint32_t result = 1;
	while(exponent--)
	{
		result *= base;
	}
	return result;
}