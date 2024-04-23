/*******************************************************************************/
/**
* @file PORT.h
* @brief PORT implementation in C language
*
* @par Project Name
* atmega32 MCAL layer
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
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef PORT_H_
#define PORT_H_
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
#include "PORT_CONFIG.h"
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

/*----------------------------------------------------------------------------*/
/**
*	@brief	This enum contain all DIO ports in Atmega32 micro-controller
*			if you need to access port A you should use PORT_enmPortA etc.
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
	
	/**
	* @brief PORT_enmPortA used to access PortA.
	*/
	PORT_enmPortA,
	
	/**
	* @brief  PORT_enmPortB used to access PortB.
	*/
	PORT_enmPortB,
	
	/**
	* @brief  PORT_enmPortC used to access PortC.
	*/
	PORT_enmPortC,
	
	/**
	* @brief  PORT_enmPortD used to access PortD.
	*/
	#if NUMBER_OF_PORTS == 4
	PORT_enmPortD
	#endif
	
} PORT_enmPortOPTS_t;


/*----------------------------------------------------------------------------*/
/**
 * @brief This enum contain all Errors which may be occur during using 
 *		  PORT driver it's useful .
*/
/*----------------------------------------------------------------------------*/
typedef enum 
{
	/**
	*@brief  Function executed with no errors.
	*/
	PORT_enmOk,             
	
	/**
	*@brief you entered a pin out of the range it is only from 1 to 8 . 
	*/
	PORT_enmPinNumOutOfRange,
	
	/**
	*@brief you entered an invalid port .  
	*/
	PORT_enmPortInvalid,
	
	/**
	* @brief Error in pin configuration allowed only with ( PORT_enmOutput , PORT_enumInputInternalPullUp 
	*														, PORT_enumInputExternalPullDown)
	*/
	PORT_enmInValidPinConf ,
	
	/**
	* @brief  Error. Entered Invalid Pin state
	*/
	PORT_enmInvalidPinState ,
	
	/**
	* @brief  Error. The pin is input not input
	*/
	PORT_enmPinIsInputNotOutput ,
	
	/**
	* @brief  Error. The port is Input , it must be output
	*/
	PORT_enmPortIsInputNotOutput ,
	
	/**
	* @brief  Error. Entered Invalid Port state
	*/
	PORT_enmInvalidPortState ,
	
	/**
	* @brief  Error. Entered pin is output is not input occur when reading output pin
	*/
	PORT_enmPinIsOutputNotInput ,
	/**
	* @brief  Error. you don't pass an address for variable .
	*/
	PORT_enmNullPtrPassed
	
} PORT_enmError_t;


/**
*@brief		This enum contain all configurations to DIO pins 
*			you should using it to configure the pin is input
*			or output.
*		 
*/
typedef enum
{	
	/**
	*@brief		you should use PORT_enmOutput_High when you want to 
	*			make pin act as output pin (EX. you want to 
	*			turn on Led). and the default will be HIGH in Active High.
	*/
	PORT_enmOutputHigh,
	
	/*
	*@brief		you should use PORT_enmOutput_LOW when you want to
	*			make pin act as output pin (EX. you want to
	*			turn on Led). and the default will be LOW in Active High mode.
	*/
	PORT_enmOutputLOW,
	

	/**
	 *@brief	you should use PORT_enumInputInternalPullUp when 
	 *			you want to make pin act as input pin and Pull-up 
	 *			will activate (EX. you wait input from button) .
	 */           
	PORT_enumInputInternalPullUp,

	/**
	 *@brief	you should use PORT_enumInputExternalPullDown when 
	 *			you want to make pin act as input pin and Pull-up 
	 *			will not activate (EX. you wait input from button) .
	 */      
	PORT_enumInputExternalPullDown
	
} PORT_enmConfig_t;


/*----------------------------------------------------------------------------*/
/**
*@brief		This enum contain all status for the pins which
*			configured as output pin ONLY!
*		 
*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/**
 * @brief	This enum contain all pins you can
 *			use in each PORT ports (A,B,C,D).
 *		 
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
	/**
	*@brief		you should use PORT_enumPin1 if you want access the first pin
	*			in each DIO ports (A,B,C,D).
	*/
	PORT_enumPin1,
	
	/**
	*@brief		you should use PORT_enumPin2 if you want access the second pin
	*			in each DIO ports (A,B,C,D).
	*/
	PORT_enumPin2,
	
	/**
	*@brief		you should use PORT_enumPin3 if you want access the third pin
	*			in each DIO ports (A,B,C,D).
	*/
	PORT_enumPin3,
	
	/**
	*@brief		you should use PORT_enumPin4 if you want access the forth pin
	*			in each DIO ports (A,B,C,D).
	*/
	PORT_enumPin4,
	
	/**
	 *@brief	you should use PORT_enumPin5 if you want access the fifth pin
	 *			in each PORT ports (A,B,C,D).
	*/
	PORT_enumPin5,
	
	/**
	*@brief		you should use PORT_enumPin6 if you want access the sixth pin
	 *			in each DIO ports (A,B,C,D).
	 */
	PORT_enumPin6,
	
	/**
	 *@brief	you should use PORT_enumPin7 if you want access the seventh pin
	 *			in each DIO ports (A,B,C,D).
	 */
	PORT_enumPin7,
	
	/**
	*@brief		you should use PORT_enumPin6 if you want access the eighth pin
	*			in each DIO ports (A,B,C,D).
	*/
	PORT_enumPin8
	
} PORT_enumPins_t;
/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/

/*
*	@brief	this struct used to configure pins , you can pass it to PORT_enmSetConf as argument
*/
typedef struct 
{
	/*
	*@brief you can use it to determine the port 
	*		you can use PORT_enmPortOPTS_t enum
	*		to determine the specific port.
	*		EX:-
	*		PORT_enmPortA
	*		PORT_enmPortB
	*		..
	*/
	PORT_enmPortOPTS_t	enmPort;
	
	/*
	*@brief you can use it to determine th pin
	*		which you want to use you can use 
	*		PORT_enumPins_t enum to determine 
	*		the pin.
	*		Ex:-
	*		PORT_enumPin1
	*		PORT_enumPin2
	*		PORT_enumPin3
	*
	*/
	PORT_enumPins_t		enmPin;
	
	/*
	*@brief you can use it to determine the con-
	*		figuration of the pin you can use P-
	*		ORT_enmConfig_t enum.
	*		EX:- PORT_enmOutputHigh
	*		PORT_enmOutputLOW
	*		PORT_enumInputInternalPullUp
	*
	*/		
	PORT_enmConfig_t	enmPinConf;
	
} PORT_stPortCfg_t;

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

/*
* @brief		this function used to Set configuration in the pin which you determine. 
*
* @param[in]	ADD_cpstPortConf	this is struct used to configure the pin. the struct
*									contain pin, port , and configuration (EX:-outputHigh) 
*																	
*/
PORT_enmError_t PORT_enmSetCfg(const PORT_stPortCfg_t * const ADD_cpstPortConf);

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
#endif /* PORT_H_ */
/******************************************************************************/
