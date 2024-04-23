/*******************************************************************************/
/**
 * @file Interrupt.h
 * @brief This header file contains APIs and macros for controlling both external
 * and peripheral interrupts on the AVR Atmega32 microcontroller.
 *
 * @par Project Name
 * Atmega32 Microcontroller Abstraction Layer (MCAL)
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This header file provides a comprehensive set of functions to initialize,
 * enable, disable, and set callback functions for both external and peripheral 
 * interrupts on the Atmega32 microcontroller. It abstracts the low-level hardware
 * configurations and interactions, making it easier for higher-level application 
 * code to handle interrupt-driven events.
 *
 * The external interrupts on the Atmega32 are supported by EXINT0 and EXINT1 pins,
 * and this API allows users to easily configure and manage these interrupts.
 *
 *
 * This file is part of the Atmega32 Microcontroller Abstraction Layer (MCAL) project, 
 * aiming to provide a clean and modular interface for interacting with the Atmega32
 * microcontroller at the hardware level.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/


/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef EXINTERRUPT_H_
#define EXINTERRUPT_H_
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

/*include the platform types */
#include "../../00_LIB/Platform_Types.h"
#include "Interrupt_CFG.h"
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
* @brief this all functions error status which will be returned.
*/
typedef enum
{
	/**
	*@brief returned if the function done it task correctly.
	*/
	EXINT_enuOK,
	
	/**
	*@brief returned if the Interrupt name is invalid or not activated.
	*/
	EXINT_enuNotValidIntName,
	
	/**
	*@brief  returned if null pinter to function passed
	*/
	EXINT_enuNullPtr
	
} EXINT_enuError;


/**
*@brief contain all interrupts names which you can to attach call-back 
*		function with certain interrupt. 
*/
typedef enum
{
	#if INT0 == ENABLE		/*Check if the EXEXINT0 interrupt enabled in Interrupt_CFG.h */
	/**
	* @brief refer to External Interrupt Request 0 interrupt.
	*/
	EXINT_enuEXINT0,		
	
	#elif INT0 != DISABLE	/*Check if wrong configuration is chose  in Interrupt_CFG.h */
		#error the INT0 configuration is wrong it should be  (ENABLE , DISABLE )
	#endif

	#if INT1 == ENABLE	/*Check if the EXINT1 interrupt enabled in Interrupt_CFG.h */
	/**
	* @brief refer to External Interrupt Request 1 interrupt.
	*/
	EXINT_enuEXINT1,
	
	#elif INT1 != DISABLE	/*Check if wrong configuration is chose  in Interrupt_CFG.h */
		#error the INT1 configuration is wrong it should be  (ENABLE , DISABLE )
	#endif
	
	#if INT2 == ENABLE	/*Check if the EXINT2 interrupt enabled in Interrupt_CFG.h */
	/**
	* @brief refer to External Interrupt Request 2 interrupt.
	*/
	EXINT_enuEXINT2,
	
	#elif INT2 != DISABLE	/*Check if wrong configuration is chose  in Interrupt_CFG.h */
		#error the INT2 configuration is wrong it should be  (ENABLE , DISABLE )
	#endif
	
} EXINT_enuInteruptsNames;

/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/

/**
* @brief		this function used to Set configuration for atmega32 interrupts.
*
* @return		No thing
*
* @note			you should configure the interrupt first.
*/
void EXINT_init(void);



/**
* @brief							  this function used to set callback function which will called 
*									  when the interrupt occur.
*
*
* @param[in] Copy_InterruptName       Determines the name of the interrupt. 
*									  When triggered, this interrupt will
*									  call the specified callback.
*
*
* @param[in] ADD_CallBack			 Contain the address of the function which called when interrupt occur. 	
* 
*
*
* @param[in] ADD_vpParam			generic pointer to the any data type. which you can use. 	
*
*
* @note								you will not be able to pass callback to disabled interrupt.
*/
EXINT_enuError EXINT_enuSetCallBack(EXINT_enuInteruptsNames Copy_InterruptName, 
								void (*ADD_CallBack)(void*),void * ADD_vpParam);

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
#endif /* FILE_H_ */
/******************************************************************************/
