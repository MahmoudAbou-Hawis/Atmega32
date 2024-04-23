/******************************************************************************/
/**
 * @file DIO.c
 * @brief this is the implementation of DIO driver
 *
 * @par Project Name
 * DIO driver
 *
 * @par Code Language
 * C
 *
 * @par Description
 * A DIO driver is a software library that helps you control the input and output pins
 * of the ATmega32 micro-controller. With a DIO driver, you can easily set the direction
 * of each pin (input or output), read the state of each pin, and write data to each pin.
 * 
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "DIO.h"
#include "../../00_LIB/BIT_MATH.h"
#include "RegMap.h"
#include "../01_PORT/PORT_CONFIG.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/


/******************************************************************************/
/**
*@brief  The number of ports in atmega32
*/
/******************************************************************************/


/******************************************************************************/
/**
*@brief  all pins in the port is output
*/
/******************************************************************************/
#define PORT_IS_OUTPUT				0xFF


/******************************************************************************/
/**
*@brief  frequency of the clock
*/
/******************************************************************************/
#ifndef F_CPU
#define F_CPU   1000000UL
#endif

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
/******************************************************************************/

 
/******************************************************************************/
 
 
/******************************************************************************/

/******************************************************************************/
/* PRIVATE ENUMS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE TYPES */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/
/**
*@brief this is constant array of all registers in DIO .
*
/******************************************************************************/

static DIOPortsReg_t* const cpstDIOPortReg[NUMBER_OF_PINS_IN_EACH_PORT] = {PORTA , PORTB , PORTC 
	
	#if NUMBER_OF_PORTS == 4
	, PORTD
	#endif
};


#if NUMBER_OF_PORTS == 4
#define  LAST_PORT      3
#elif NUMBER_OF_PORTS == 3
#define  LAST_PORT  2
#endif

/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLE DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/

DIO_enumError_t DIO_enumSetPin(DIO_enmPortOPTS Copy_enmPortNum,DIO_enumPins_t Copy_enmPinNum,DIO_enmLOutputPinState_t Copy_enmLogicState)
{
	/* Validation */
	DIO_enumError_t Ret_enmErrorStatus = DIO_enmOk;												  /*The Error State code*/
	if(Copy_enmPortNum > LAST_PORT || Copy_enmPortNum < DIO_enmPortA)				  		  /*Check if the Port Entered in the range or not*/
	{
		Ret_enmErrorStatus = DIO_enmPortInvalid;												  /*Return will be Not valid port*/
	}
	else if(Copy_enmPinNum > DIO_enumPin8 || Copy_enmPinNum < DIO_enumPin1 )				      /*Check if the pin Entered in the range or not*/
	{
		Ret_enmErrorStatus = DIO_enmPinNumOutOfRange;											  /*Return will be Pin out of the allowed range*/
	}
	else if(Copy_enmLogicState > DIO_enumLogicHigh || Copy_enmLogicState < DIO_enumLogicLow)	  /*check if the Logic state is invalid*/
	{
		Ret_enmErrorStatus = DIO_enmInvalidPinState;											  /*return will be Error in Pin status*/
	}
	else if(!GET_BIT(cpstDIOPortReg[Copy_enmPortNum]->u8DDR,Copy_enmPinNum))			   		 /*Check if the pin direction is input*/
	{
		Ret_enmErrorStatus = DIO_enmPinIsInputNotOutput;                                          /*return will be the pin is input can't execute*/
	} 
	else
	{
		/*implementation*/
		switch(Copy_enmLogicState)
		{
			case DIO_enumLogicLow:											                          /*if the Copy_enmLogicState state is low*/
				 CLR_BIT(cpstDIOPortReg[Copy_enmPortNum]->u8PORT , Copy_enmPinNum);				      /*make the pin state is low */
				 break;
			case DIO_enumLogicHigh:																	 /*if the Copy_enmLogicState state is High*/
				 SET_BIT(cpstDIOPortReg[Copy_enmPortNum]->u8PORT , Copy_enmPinNum);					 /*make the pin state is high */
				 break;
			default:																				 /*otherwise doesn't make any thing*/
				break;																				 
		}
	}
	return Ret_enmErrorStatus;																	/*return the function execution state*/
}

DIO_enumError_t DIO_enumSetPort(DIO_enmPortOPTS Copy_enmPortNum,DIO_enmLOutputPinState_t Copy_enmLogicState)
{
	/* Validation */
	DIO_enumError_t Ret_enmErrorStatus = DIO_enmOk;												  /*The Error State code*/
	if(Copy_enmPortNum > LAST_PORT || Copy_enmPortNum < DIO_enmPortA)				  		  /*Check if the Port Entered in the range or not*/
	{
		Ret_enmErrorStatus = DIO_enmPortInvalid;												  /*Return will be Not valid port*/
	}
	else if(Copy_enmLogicState > DIO_enumLogicHigh || Copy_enmLogicState < DIO_enumLogicLow)	  /*check if the Logic state is invalid*/
	{
		Ret_enmErrorStatus = DIO_enmInvalidPortState;											  /*return will be Error in PORT status*/
	}
	else if(GET_REG(cpstDIOPortReg[Copy_enmPortNum]->u8DDR) != PORT_IS_OUTPUT)			     	  /* if PORT is Not OUTPUT */
	{
		Ret_enmErrorStatus = DIO_enmPortIsInputNotOutput;
	}
	else
	{
		/*implementation*/
		switch(Copy_enmLogicState)
		{
			case DIO_enumLogicLow:											                         /*if the Copy_enmLogicState state is low*/
				CLR_REG(cpstDIOPortReg[Copy_enmPortNum]->u8PORT);			   				  	     /*make the port pins state is low */
				break;
			case DIO_enumLogicHigh:																	 /*if the Copy_enmLogicState state is High*/
				SET_REG(cpstDIOPortReg[Copy_enmPortNum]->u8PORT );			    					 /*make the port pins state is high */
				break;
			default:																		     /*otherwise doesn't make any thing*/
				break;
		}
	}
	return Ret_enmErrorStatus;
}


DIO_enumError_t DIO_enumGetState(DIO_enmPortOPTS Copy_enmPortNum,DIO_enumPins_t Copy_enmPinNum,uint8_t* Add_Pu8PinValue)
{
	/* Validation */
	DIO_enumError_t Ret_enmErrorStatus = DIO_enmOk;												  /*The Error State code*/
	if(Copy_enmPortNum > LAST_PORT || Copy_enmPortNum < DIO_enmPortA)				  		  /*Check if the Port Entered in the range or not*/
	{
		Ret_enmErrorStatus = DIO_enmPortInvalid;												  /*Return will be Not valid port*/
	}
	else if(Copy_enmPinNum > DIO_enumPin8 || Copy_enmPinNum < DIO_enumPin1 )				      /*Check if the pin Entered in the range or not*/
	{
		Ret_enmErrorStatus = DIO_enmPinNumOutOfRange;											  /*Return will be Pin out of the allowed range*/
	}
	else if(GET_BIT(cpstDIOPortReg[Copy_enmPortNum]->u8DDR, Copy_enmPinNum))					  /* Check if the pin output*/
	{
		Ret_enmErrorStatus = DIO_enmPinIsOutputNotInput;
	}
	else if(Add_Pu8PinValue == NULL)																/*Check if Add_Pu8PinValue equal NULL*/
	{
		Ret_enmErrorStatus = DIO_NullPtrPassed;
	}
	else
	{
		/*Implementation*/
		*Add_Pu8PinValue = GET_BIT(cpstDIOPortReg[Copy_enmPortNum]->u8PIN , Copy_enmPinNum);       /*the value of the input pin (HIGH or LOW) */
	}
	return Ret_enmErrorStatus;
}


/******************************************************************************/