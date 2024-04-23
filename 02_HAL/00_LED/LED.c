/*******************************************************************************/
/**
 * @file LED.h
 * @brief this file contain all functions which can used to control led
 *
 * @par Project Name
 * Led driver
 *
 * @par Code Language
 * C
 *
 * @par Description
 *	this file can used for control all LEDs which you configured in LED_CFG.h and LED_CFG.c
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "../../01_MCAL/00_DIO/DIO.h"
#include "../../01_MCAL/01_PORT/PORT.h"
#include "LED.h"
#include "LED_CFG.h"

/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/
#define	ONLY_LED_CONNECTED			0

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
extern const LED_stLedCFG LED_caLedsCFG[NUMBER_OF_LEDS];

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

LED_enmErrors_t LED_enmInit(void)
{
	LED_enmErrors_t RET_enmErrorStatus	= LED_enmOK;			/*return of the function initialize with LED_enmOK*/
	PORT_enmError_t LOC_enmErrorStatus	= PORT_enmOk;			/*Local return status error form function PORT_enmSetCfg() */
	PORT_enmConfig_t LOC_enuPinCfg		=  PORT_enmOutputHigh;	/*contain the default pin state as output */
	PORT_stPortCfg_t LOC_stPinCFG		= {0};					/*contain default pin Configuration like port and pin*/
	boolean LOC_bExit					= FALSE;				/*variable for exit the for loop , following MISRA*/
	for(uint8_t LOC_u8led = 0 ; LOC_u8led < NUMBER_OF_LEDS && !LOC_bExit ; LOC_u8led++)
	{
		if(LED_caLedsCFG[LOC_u8led].enmActivationState > LED_enuActiveHigh	/*check if the led has invalid state*/
		|| LED_caLedsCFG[LOC_u8led].enmActivationState < LED_enuActiveLow)
		{
			RET_enmErrorStatus	= LED_enmInvalidActivationState;			/*returned value is invalid activation state*/
			LOC_bExit			= TRUE;										/*exist the led initialization process*/
		}
		else if(LED_caLedsCFG[LOC_u8led].enmLedCfg > LED_enmOn				/*check if the led has a bad opts( high or low) */
			|| LED_caLedsCFG[LOC_u8led].enmLedCfg < LED_enmOff)
		{
			RET_enmErrorStatus	= LED_enmInvalidLedState;					/*return the led state is invalid*/	
			LOC_bExit			= TRUE;										/*exist the led initialization process*/
		}
		else
		{
			if((LED_caLedsCFG[LOC_u8led].enmActivationState == LED_enuActiveHigh /*if the led has good configuration and high*/
				&& LED_caLedsCFG[LOC_u8led].enmLedCfg == LED_enmOn)
				||(LED_caLedsCFG[LOC_u8led].enmActivationState == LED_enuActiveLow 
				&& LED_caLedsCFG[LOC_u8led].enmLedCfg == LED_enmOff)
			)
			{
				LOC_enuPinCfg = PORT_enmOutputHigh;									/*the pin port is high*/
			}
			else
			{
				LOC_enuPinCfg = PORT_enmOutputLOW;									/*the pin port is low*/
			}
			
			LOC_stPinCFG.enmPin  = LED_caLedsCFG[LOC_u8led].enmPin;					/*init the configuration led for the port */
			LOC_stPinCFG.enmPort = LED_caLedsCFG[LOC_u8led].enmPort;
			LOC_stPinCFG.enmPinConf = LOC_enuPinCfg;
			
			LOC_enmErrorStatus = PORT_enmSetCfg(&LOC_stPinCFG);						/*Trying to set the pin configuration*/
			if(LOC_enmErrorStatus == PORT_enmPortInvalid)							/*check if the port is invalid*/
			{
				RET_enmErrorStatus = LED_enmInvalidPort;							/*return invalid status*/
				LOC_bExit = TRUE;													/*will exit the the init process*/
			}
			else if(LOC_enmErrorStatus == PORT_enmPinNumOutOfRange)					/*check if the pin out of valid range */
			{
				RET_enmErrorStatus = LED_enmInvalidPin;								/*return the pin is not valid*/
				LOC_bExit = TRUE;													/*exit the init process*/
			}
			else
			{
				/*Do nothing*/
			}
		}
	}
	return RET_enmErrorStatus;														/*return function execution status*/
}



LED_enmErrors_t LED_enmSetLedState(const uint8_t Copy_u8Led,const LED_enmLedState_t Copy_cenmState)
{
	/*Validation*/
	LED_enmErrors_t RET_enmErrorStatus = LED_enmOK;								/*the function return initialization */
	DIO_enumError_t LOC_enmErrorStaus = DIO_enmOk;								/*the return value from function DIO_enumSetPin*/	
	if(Copy_u8Led >= NUMBER_OF_LEDS 
	|| Copy_u8Led < ONLY_LED_CONNECTED)											/*check if the led is already exist */
	{		
		RET_enmErrorStatus = LED_InvalidLed;									/*the function return will be invalid pin*/
	}
	else if(Copy_cenmState < LED_enmOff || Copy_cenmState > LED_enmOn)			/*Check if the valid state passed*/
	{
		RET_enmErrorStatus = LED_enuInvalidState;								/*return the invalid state error*/
	}
	else
	{
		DIO_enmLOutputPinState_t LOC_enmLedState =								/*Determine the pin state*/
													((
														LED_caLedsCFG[Copy_u8Led].enmActivationState == LED_enuActiveHigh
														&& 	Copy_cenmState == LED_enmOn				
													) || (
													LED_caLedsCFG[Copy_u8Led].enmActivationState == LED_enuActiveLow 
													&& Copy_cenmState == LED_enmOff
													))?DIO_enumLogicHigh: DIO_enumLogicLow;
													
		LOC_enmErrorStaus = DIO_enumSetPin(LED_caLedsCFG[Copy_u8Led].enmPort,	/*Trying to set pin is high*/
											LED_caLedsCFG[Copy_u8Led].enmPin,LOC_enmLedState);
		if(LOC_enmErrorStaus == DIO_enmPinIsInputNotOutput)						/*if the DIO_enumSetPin Failed to set the pin*/
		{
			RET_enmErrorStatus = LED_ConfiguredInputInAnotherModule;			/*The pin is INPUT not OUTPUT*/
		}
		else
		{
			/* No Thing*/
		}
	}
	return RET_enmErrorStatus;													/*the function execution status return */				
}



/******************************************************************************/