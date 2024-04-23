/*
 * RegMap.h
 *
 * Created: 12/3/2023 6:02:07 PM
 *  Author: Mahmoud Sofar
 */ 


#ifndef REGMAP_H_
#define REGMAP_H_

#include "../../00_LIB/Platform_Types.h"

/******************************************************************************/
/**
*@brief  The base address of PORT A registers
*/
/******************************************************************************/
#define PORTA ((DIOPortsReg_t*)0x39)

/******************************************************************************/
/**
*@brief  The base address of PORT B registers
*/
/******************************************************************************/
#define PORTB ((DIOPortsReg_t*)0x36)

/******************************************************************************/
/**
*@brief  The base address of PORT C registers
*/
/******************************************************************************/
#define PORTC ((DIOPortsReg_t*)0x33)

/******************************************************************************/
/**
*@brief  The base address of PORT D registers
*/
/******************************************************************************/
#define PORTD ((DIOPortsReg_t*)0x30)

/******************************************************************************/
/**
*@brief  This struct access all registers  in each port 
*/
/******************************************************************************/
typedef struct
{
	
	/**
	* @brief PINA is an register used to get the status of pin if it set as INPUT.
	*/
	volatile uint8_t u8PIN;
	
	/**
	* @brief DDR is an register used to determine the direction of pin (INPUT or OUTPUT).
	*/
	volatile uint8_t u8DDR;
	
	/*
	* @brief PORT is an register used to set the pins in PORT  (High or low)
	*		 if the pin is OUTPUT , if the pin is input it determine the pull-up
	*		 activated or not .
	*/
	volatile uint8_t u8PORT;
	
} DIOPortsReg_t;


#endif /* REGMAP_H_ */