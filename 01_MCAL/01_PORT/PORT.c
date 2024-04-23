/*******************************************************************************/
/**
* @file PORT.h
* @brief PORT implementation in C language
*
* @par Project Name
* PORT driver for Atmega32 micro-controller.
*
* @par Code Language
* C
*
* @par Description
* A PORT driver is a software library that helps you to configure all pins in atmega32
*
* @par Author
* Mahmoud Abou-Hawis
*
 ******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "PORT.h"
#include "RegMap.h"
#include "../../00_LIB/BIT_MATH.h"
#include "../../00_LIB/Platform_Types.h"
#include "PORT_CONFIG.h"

/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/

#define PORT_D				3		/*Last PORT is PORT D */

#define PORT_C				2		/*Last PORT is PORT C*/


#if NUMBER_OF_PORTS == 4			/*NUMBER_OF_PORTS May be 4 or 3 ports */
#define  LAST_PORT      PORT_D		/*if you choose NUMBER_OF_PORTS is 4 so LAST_PORT IS PORT D */
#elif NUMBER_OF_PORTS == 3
#define  LAST_PORT  PORT_C			/*if you choose NUMBER_OF_PORTS is 3 so LAST_PORT IS PORT C */
#endif

/******************************************************************************/


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
*/
/******************************************************************************/
#if NUMBER_OF_PORTS == 4	/* if you configured the NUMBER_OF_PORTS in POR_CONFIG.h
							as 4 , will define 4 ports */
/**
*@brief this is array of constant pointers for 4 ports if you configured NUMBER_OF_PORTS
*		as 4 ports
*/ 
static DIOPortsReg_t* const cpstDIOPortRegs[NUMBER_OF_PORTS] = {PORTA , PORTB , PORTC , PORTD};
#elif NUMBER_OF_PORTS == 3	/* if you configured the NUMBER_OF_PORTS in POR_CONFIG.h as 3 , will define 3 ports */
/**
*@brief this is array of constant pointers for 3 ports if you configured NUMBER_OF_PORTS
*		as 3 ports
*/
static DIOPortsReg_t* const cpstDIOPortRegs[NUMBER_OF_PORTS] = {PORTA , PORTB , PORTC};
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

/*----------------------------------------------------------------------------*/
/**
 * @brief							 this function used to configure the pin (Input/Output).
 * 
 * @param[in] Copy_enmPortNum		this attribute used to determine the port which you want to use
 *									(EX. PORT_enmPortA)  , allowed only PORT_enmPortA ,PORT_enmPortB
 *									PORT_enmPortC , PORT_enmPortD.
 *
 * @param[in] Copy_enmPinNum		refer to the pin which you want to use like (PORT_enumPin1) you
 *									have only 8 pins in each port . the pins defined in PORT_enumPins_t
 *									enum.
 * 									  
 * @param[in] Copy_enmConfig		this attribute used for determine the pin is input or output
 *									(EX. PORT_enmOutput) all configurations pre-defined in PORT_enmConfig_t 
 *									enum.
 *
 * @return enum contain Error State
 * 
 * @note							  you must use this function before any operation in the pin.
/*----------------------------------------------------------------------------*/
static PORT_enmError_t enmSetPinConfig(const PORT_enmPortOPTS_t Copy_enmPortNum,const PORT_enumPins_t Copy_enmPinNum
																	,const PORT_enmConfig_t Copy_enmConfig);

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

static PORT_enmError_t enmSetPinConfig(const PORT_enmPortOPTS_t Copy_enmPortNum,const PORT_enumPins_t Copy_enmPinNum
																				,const PORT_enmConfig_t Copy_enmConfig)
{
	
	/* Validation */	
	PORT_enmError_t Ret_enmErrorStatus = PORT_enmOk;						/*The Error State code*/
	if(Copy_enmPortNum > LAST_PORT || Copy_enmPortNum < PORT_enmPortA)		/*Check if the Port Entered in the range or not*/
	{
		Ret_enmErrorStatus = PORT_enmPortInvalid;							/*Return will be Not valid port*/
	}
	else if(Copy_enmPinNum > PORT_enumPin8									/*Check if the pin Entered in the range or not*/
			|| Copy_enmPinNum < PORT_enumPin1 )	
	{
		Ret_enmErrorStatus = PORT_enmPinNumOutOfRange;						/*Return will be Pin out of the allowed range*/
	}
	else if(Copy_enmConfig > PORT_enumInputExternalPullDown					/*check if pin configuration right*/
		   || Copy_enmConfig < PORT_enmOutputHigh)	
	{
		Ret_enmErrorStatus = PORT_enmInValidPinConf;						/*Return Invalid Configuration*/
	}
	else
	{
		/*Implementation*/
		switch(Copy_enmConfig)												/*switch in the Configurations*/
		{
			case PORT_enmOutputHigh:
			SET_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8DDR					/*direction of the pin in selected port is output*/
											,Copy_enmPinNum);
			SET_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8PORT					/*this make the the pin in HIGH state */
			,Copy_enmPinNum);
			break;
			 
			case PORT_enmOutputLOW:
			SET_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8DDR					/*direction of the pin in selected port is output*/
											,Copy_enmPinNum);	
			CLR_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8PORT					/*this make the the pin in low state */
											,Copy_enmPinNum);
			break;
			case PORT_enumInputInternalPullUp:
			CLR_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8DDR					/*direction of the pin in selected port is input*/
											,Copy_enmPinNum);				  
			SET_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8PORT					/*the pull-up activated in selected pin */
											,Copy_enmPinNum);			 
			break;
			case PORT_enumInputExternalPullDown:
			CLR_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8DDR					/*direction of the pin in selected port is input*/
											,Copy_enmPinNum);				  
			CLR_BIT(cpstDIOPortRegs[Copy_enmPortNum]->u8PORT					/*the pull-up disabled in selected pin */
											,Copy_enmPinNum);			      
			break;
			default:														/*otherwise nothing yo do*/
			break;
		}
		
	}
	return Ret_enmErrorStatus;												/*return the function execution state*/
	
}


/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/
PORT_enmError_t PORT_enmSetCfg(const PORT_stPortCfg_t * const ADD_cpstPortConf)
{
	PORT_enmError_t Ret_enmErrorStatus = PORT_enmOk;							/*contain the error which you may be occur 
																				all errors in PORT_enmError_t if no error it 
																				will be OK*/
	
	Ret_enmErrorStatus = enmSetPinConfig(ADD_cpstPortConf->enmPort				/*set the pin conf and return Error state */
									,ADD_cpstPortConf->enmPin, ADD_cpstPortConf->enmPinConf);
	return Ret_enmErrorStatus;													/*return the error status*/
}
/******************************************************************************/