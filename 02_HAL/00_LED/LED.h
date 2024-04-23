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
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef LED_H_
#define LED_H_
/******************************************************************************/

/******************************************************************************/
/* C++ Style GUARD */
/******************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
/******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "../../00_LIB/Platform_Types.h"
#include "LED_CFG.h"
/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PUBLIC MACROS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC ENUMS */
/******************************************************************************/

/**
* @brief this enum contain all errors may occur
*/
typedef enum 
{
	/**
	*@brief  if the function done correctly
	*/
	LED_enmOK,
	
	/**
	*@brief if you entered wrong port while configuration 
	*/
	LED_enmInvalidPort,
	
	/**
	*@brief if you entered wrong pin while configuration 
	*/
	LED_enmInvalidPin,
	
	/**
	*@brief if you entered wrong Activation state while configuration .
	*/
	LED_enmInvalidActivationState,
	
	/**
	*@brief if you entered wrong Invalid Led state while configuration .
	*/
	LED_enmInvalidLedState ,
	
	/**
	*@brief if you entered wrong Led which not configured .
	*/
	LED_InvalidLed ,
	
	/**
	*@brief if you configured the pin is input in another module .
	*/
	LED_ConfiguredInputInAnotherModule,
	
	/**
	*@brief if the invalid led state passed as argument in function
	* LED_enmSetLedState , it must be on or off only.
	*/
	LED_enuInvalidState 
	
} LED_enmErrors_t;


/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DECLARATIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLE DECLARATIONS */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION PROTOTYPES */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/**
 * @brief							used to initialize all LEDS which you configured in LED_CFG.h
 * 	
 * @return							enum contain Error State
 * 
 * @note							you must configure all LEDs in LED_CFG.h.
/*----------------------------------------------------------------------------*/
LED_enmErrors_t LED_enmInit(void);

/*----------------------------------------------------------------------------*/
/**
 * @brief							used to make the led on.
 * 
 * @param[in] Copy_u8Led			led id which you want to choose 
 * @param[in] Copy_cu8State			to determine the led is ON or OFF
 *
 * @return							enum contain Error State
 * 
 * @note							you must use this function after configuring the led.
/*----------------------------------------------------------------------------*/
LED_enmErrors_t LED_enmSetLedState(const uint8_t Copy_u8Led,const LED_enmLedState_t Copy_cenmState);

/******************************************************************************/

/******************************************************************************/
/* C++ Style GUARD */
/******************************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */
/******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#endif /* LED_H_ */
/******************************************************************************/
