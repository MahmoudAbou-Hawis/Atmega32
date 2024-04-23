/*******************************************************************************/
/**
 * @file SWITCH.h
 * @brief this is the switch driver which contain all function which you want to 
 * to read the switch status and initialize all of switches
 *
 * @par Project Name
 * switch driver
 *
 * @par Code Language
 * C
 *
 * @par Description
 * this driver used to read status of each switch in the whole project
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "../../01_MCAL/01_PORT/PORT.h"
#include "../../01_MCAL/00_DIO/DIO.h"
#include "SWITCH_CFG.h"
#include "SWITCH.h"

/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/
#define  ONLY_SWITCH_CONNECTED			0

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
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

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DEFINITIONS */
/******************************************************************************/
extern const SWITCH_stSqitchCFG_t SWITCH_caSwitchesCFG[NUMBER_OF_SWITCHS];

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

SWITCH_enmErrors_t SWITCH_enmInit(void)
{
	/*Validation*/
	SWITCH_enmErrors_t RET_enuErrorStatus = SWITCH_enuOK;									/*function return status initialized with No Error */
	PORT_enmError_t LOC_enuErrorStatus = PORT_enmOk;										/*this is the return status of PORT_enmSetCfg function*/
	boolean LOC_bExit = FALSE;																/*to exist the loop if the function execution not success*/
	PORT_stPortCfg_t LOC_stPinCFG		= {0};												/*contain default pin Configuration like port and pin*/
	for(int LOC_Switch = 0; LOC_Switch  < NUMBER_OF_SWITCHS && !LOC_bExit; LOC_Switch++)	/*Loop in each configured switch*/
	{
		if(SWITCH_caSwitchesCFG[LOC_Switch].enmSwitchState > SWITCH_enmPullUp ||			/*Check if Switch has bad Status should be(pullup/pulldown)*/
			SWITCH_caSwitchesCFG[LOC_Switch].enmSwitchState < SWITCH_enmPullDown )
		{
				RET_enuErrorStatus = SWITCH_enuBadState;									/*return function will be bad switch state*/
				LOC_bExit			= TRUE;													/*Exist the Switches Initialization*/
		}
		else
		{
			LOC_stPinCFG.enmPin  = SWITCH_caSwitchesCFG[LOC_Switch].enmPin;					/*set pin configuration*/
			LOC_stPinCFG.enmPort = SWITCH_caSwitchesCFG[LOC_Switch].enmPort;				/*set port configuration*/
			if(SWITCH_caSwitchesCFG[LOC_Switch].enmSwitchState == SWITCH_enmPullUp)			/*check if the button state is pull-up*/
			{
				LOC_stPinCFG.enmPinConf = PORT_enumInputInternalPullUp;						/*set the pin configuration as input Pull up*/
			}
			else
			{
				LOC_stPinCFG.enmPinConf = PORT_enumInputExternalPullDown;					/*set the pin configuration as input Pull Down*/
			}
			LOC_enuErrorStatus = PORT_enmSetCfg(&LOC_stPinCFG);								/*set the port configuration*/
			if(LOC_enuErrorStatus == PORT_enmPortInvalid)									/*check if the port in not valid*/
			{
				RET_enuErrorStatus = SWITCH_enuInvalidPort;									/*return the port which configured is invalid*/
				LOC_bExit			= TRUE;													/*exit the loop*/
			}
			else if(LOC_enuErrorStatus == PORT_enmPinNumOutOfRange)
			{	
				RET_enuErrorStatus	= SWITCH_enuInvalidPin;									/*return the port which configured is invalid*/
				LOC_bExit			= TRUE;													/*exit the loop*/
			}
			else
			{
				/*No thing*/
			}
		}
		
	}
	return RET_enuErrorStatus;																/*the function return*/
}


SWITCH_enmErrors_t SWITCH_enmReadSwitch(const uint8_t Copy_u8SwitchId, uint8_t * const ADD_cpu8PinValue)
{
	SWITCH_enmErrors_t RET_enuErrorStatus = SWITCH_enuOK;									/*function return status initialized with No Error */
	DIO_enumError_t	LOC_enuErrorStatus	= DIO_enmOk;										/*return of the function DIO_enumGetState*/
	if(ADD_cpu8PinValue == NULL)															/*check if the ADD_cpu8PinValue is NULL*/
	{
		RET_enuErrorStatus = SWITCH_enuNullPtr;												/*function return will be pointer is equal null*/
	}
	else if(Copy_u8SwitchId >= NUMBER_OF_SWITCHS												/*Check if switch id configured or not*/
		|| Copy_u8SwitchId < ONLY_SWITCH_CONNECTED)
	{
		RET_enuErrorStatus = SWITCH_enuInvalidSwitch;										/*function return the switch is invalid*/
	}
	else
	{
		LOC_enuErrorStatus = DIO_enumGetState(SWITCH_caSwitchesCFG[Copy_u8SwitchId].enmPort,/*get the pin value */
												SWITCH_caSwitchesCFG[Copy_u8SwitchId].enmPin,ADD_cpu8PinValue);
		if(LOC_enuErrorStatus == DIO_enmPinIsOutputNotInput)								/*if the pin configured as output in another module*/
		{
			RET_enuErrorStatus = SWITCH_enuSwitchPinNotInput;								/*return switch pin configured as output in another module*/
		}
	}
	return RET_enuErrorStatus;																/*the function return*/
}

/******************************************************************************/