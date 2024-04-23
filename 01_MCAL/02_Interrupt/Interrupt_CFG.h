/*******************************************************************************/
/**
 * @file FILE.h
 * @brief Header file for interrupt configuration on the AVR Atmega32 microcontroller.
 *
 * @par Project Name
 * Atmega32 Microcontroller Abstraction Layer (MCAL)
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This header file provides macros  for configuring interrupts on the Atmega32
 * microcontroller. It abstracts the low-level hardware configurations and interactions,
 * offering a modular and clean interface for users to customize interrupt settings
 * based on their application requirements.
 *
 * The file includes macros for enabling and disabling peripheral, as well as macros 
 * for configuring specific interrupt sources. Function prototypes define the API for 
 * configuring and managing interrupts, allowing users to set up interrupt-related settings 
 * in their application code.
 *
 * This file is part of the Atmega32 Microcontroller Abstraction Layer (MCAL)  project, 
 * providing a convenient abstraction layer for interrupt handling on the Atmega32 microcontroller.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/


/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef INTERRUPT_CFG_H_
#define INTERRUPT_CFG_H_
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


/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/


/******************************************************************************/
/*				 MACROS TO CONTROL INERRUPT	              */
/******************************************************************************/

/**
* @brief used it to enable Interrupt 
*/
#define					ENABLE					0

/**
* @brief used it to disable Interrupt
*/
#define					DISABLE					1

/******************************************************************************/


/******************************************************************************/
/*            THESE MACROS FOR EXTERNAL INTARRUPT ONLY !		      */
/******************************************************************************/


/**
*@brief The falling edge generates an interrupt request.
*/
#define					FALLING_ADGE				0

/**
*@brief The rising edge generates an interrupt request.
*/
#define					RAISING_ADGE				1

/**
*@brief logical change generates an interrupt request.
*/
#define					LOGICAL_LEVEL				2

/**
*@brief low level  generates an interrupt request.
*/
#define					LOW_LEVEL				3

/******************************************************************************/


/******************************************************************************/
/*				 EXTERNAL INTERRUPT  0  CONFIGURATION			  */
/******************************************************************************/


/**
*@brief this macro used to enable or disable external interrupt INT0
* it is connected with Port D pin 2
*/
#define					INT0				ENABLE

/**
*@brief should choose interrupt type from This list [ FALLING_ADGE , RAISING_ADGE , 
*	    LOGICAL_LEVEL , LOW_LEVEL] which mentioned before . 
*
*/
#define					INT0_TYPE			FALLING_ADGE								

/******************************************************************************/

/******************************************************************************/
/*				 EXTERNAL INTERRUPT  1  CONFIGURATION					      */
/******************************************************************************/


/**
*@brief this macro used to enable or disable external interrupt INT1
* it is connected with Port D pin 3.
*/
#define					INT1				ENABLE


/**
*@brief should choose interrupt type from This list [ FALLING_ADGE , RAISING_ADGE ,
*	    LOGICAL_LEVEL , LOW_LEVEL] which mentioned before .
*
*/
#define					INT1_TYPE			FALLING_ADGE

/******************************************************************************/



/******************************************************************************/
/*				 EXTERNAL INTERRUPT  2  CONFIGURATION	      */
/******************************************************************************/

/**
*@brief this macro used to enable or disable external interrupt INT2
* it is connected with Port B pin 2.
*/
#define					INT2					DISABLE

/**
*@brief should choose interrupt type from This list [ FALLING_ADGE , RAISING_ADGE ,
*		] which mentioned before .
*
*/
#define					INT2_TYPE				FALLING_ADGE




/******************************************************************************/
/*						GENERAL CONFIGURATION								  */
/******************************************************************************/

/**
*@brief this define for number of activated  interrupts
*/
#define					NUM_OF_ACT_INTS					3



/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PUBLIC MACROS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC ENUMS */
/******************************************************************************/

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
#endif /* INTERRUPT_CFG_H_ */
/******************************************************************************/
