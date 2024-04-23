/*
 * PORT_CONFIG.c
 *
 * Created: 12/3/2023 6:38:15 PM
 *  Author: Mahmoud Sofar
 */ 
#include "PORT_CONFIG.h"
#include "../../00_LIB/Platform_Types.h"



uint8_t stPinsConfigurations[NUMBER_OF_PINS_IN_EACH_PORT * NUMBER_OF_PORTS] =
{
	OUTPUT_LOW,	/*Pin Number 1*/
	OUTPUT_LOW,	/*Pin Number 2*/
	OUTPUT_LOW,	/*Pin Number 3*/
	OUTPUT_LOW,	/*Pin Number 4*/
	INPUT_INERNAL_PULL_UP,	/*Pin Number 5*/
	DEAFULT,	/*Pin Number 6*/
	DEAFULT,	/*Pin Number 7*/
	DEAFULT,	/*Pin Number 8*/
	DEAFULT,	/*Pin Number 9*/
	DEAFULT,	/*Pin Number 10*/
	DEAFULT,	/*Pin Number 11*/
	DEAFULT,	/*Pin Number 12*/
	DEAFULT,	/*Pin Number 13*/
	DEAFULT,	/*Pin Number 14*/
	DEAFULT,	/*Pin Number 15*/
	DEAFULT,	/*Pin Number 16*/
	DEAFULT,	/*Pin Number 17*/
	DEAFULT,	/*Pin Number 18*/
	DEAFULT,	/*Pin Number 19*/
	DEAFULT,	/*Pin Number 20*/
	DEAFULT,	/*Pin Number 21*/	
	DEAFULT,	/*Pin Number 22*/
	DEAFULT,	/*Pin Number 23*/
	DEAFULT,	/*Pin Number 24*/
	#if NUMBER_OF_PORTS == 4
	DEAFULT,	/*Pin Number 25*/
	DEAFULT,	/*Pin Number 26*/
	DEAFULT,	/*Pin Number 27*/	
	DEAFULT,	/*Pin Number 28*/
	DEAFULT,	/*Pin Number 29*/
	DEAFULT,	/*Pin Number 30*/
	DEAFULT,	/*Pin Number 31*/
	DEAFULT		/*Pin Number 32*/
	#endif
};