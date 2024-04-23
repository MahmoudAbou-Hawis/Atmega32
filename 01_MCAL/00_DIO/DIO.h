/*******************************************************************************/
/**
 * @file DIO.h
 * @brief DIO implementation in C language
 *
 * @par Project Name
 * atmega32 MCAL layer.
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
 ******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef DIO_H_
#define DIO_H_
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
 * @brief This enum contain all DIO ports in Atmega32 micro-controller
 *		  if you need to access port A you should use DIO_enmPortA etc.
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
	
	/**
     * @brief DIO_enmPortA used to access PortB.
    */
	DIO_enmPortA,
	
	/**
     * @brief  DIO_enmPortB used to access PortB.
    */
	DIO_enuPortB,
	
	/**
     * @brief  DIO_enmPortC used to access PortC.
    */
	DIO_enmPortC,
	
	/**
     * @brief  DIO_enmPortD used to access PortD.
    */
	#if NUMBER_OF_PORTS == 4
	DIO_enmPortD
	#endif
} DIO_enmPortOPTS;


/*----------------------------------------------------------------------------*/
/**
 * @brief This enum contain all Errors which may be occur during using 
 *		  DIO driver it's useful .
*/
/*----------------------------------------------------------------------------*/
typedef enum 
{
	/**
	 *@brief  Function executed with no errors.
	 */
	DIO_enmOk,             
	
	/**
	*@brief you entered a pin out of the range it is only from 1 to 8 . 
	*/
	DIO_enmPinNumOutOfRange,
	
	/**
	*@brief you entered an invalid port .  
	*/
	DIO_enmPortInvalid,
	
	/**
	* @brief Error in pin configuration allowed only with ( DIO_enmOutput , DIO_enumInputInternalPullUp 
	*								      , DIO_enumInputExternalPullDown)
	*/
	DIO_enmInValidPinConf ,
	
	/**
     * @brief  Error. Entered Invalid Pin state
    */
	DIO_enmInvalidPinState ,
	
	/**
     * @brief  Error. The pin is input not input
    */
	DIO_enmPinIsInputNotOutput ,
	
	/**
     * @brief  Error. The port is Input , it must be output
    */
	DIO_enmPortIsInputNotOutput ,
	
	/**
     * @brief  Error. Entered Invalid Port state
    */
	DIO_enmInvalidPortState ,
	
	/**
     * @brief  Error. Entered pin is output is not input occur when reading output pin
    */
	DIO_enmPinIsOutputNotInput ,
	/**
     * @brief  Error. you don't pass an address for variable .
    */
	DIO_NullPtrPassed
	
} DIO_enumError_t;




/*----------------------------------------------------------------------------*/
/**
 * @brief This enum contain all status for the pins which
 *        configured as output pin ONLY!
 *		 
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
	/**
	 *@brief if pin configured as output and you want to the pin in low state(0V).
	 */
	DIO_enumLogicLow,
	
	/**
	 *@brief if pin configured as output and you want to the pin in high state (5V).
	 */
	DIO_enumLogicHigh
	
} DIO_enmLOutputPinState_t;


/*----------------------------------------------------------------------------*/
/**
 * @brief This enum contain all pins you can
 *        use in each DIO ports (A,B,C,D).
 *		 
*/
/*----------------------------------------------------------------------------*/
typedef enum
{
	/**
	 *@brief you should use DIO_enumPin1 if you want access the first pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin1,
	
	/**
	 *@brief you should use DIO_enumPin2 if you want access the second pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin2,
	
	/**
	 *@brief you should use DIO_enumPin3 if you want access the third pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin3,
	
	/**
	 *@brief you should use DIO_enumPin4 if you want access the forth pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin4,
	
	/**
	 *@brief you should use DIO_enumPin5 if you want access the fifth pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin5,
	
	/**
	 *@brief you should use DIO_enumPin6 if you want access the sixth pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin6,
	
	/**
	 *@brief you should use DIO_enumPin7 if you want access the seventh pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin7,
	
	/**
	 *@brief you should use DIO_enumPin6 if you want access the eighth pin
	 *		 in each DIO ports (A,B,C,D).
	 */
	DIO_enumPin8
	
} DIO_enumPins_t;
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
 * @brief							 if you configured the pin as OUTPUT, you should use this function
 *									 to determine if this pin is HIGH or LOW.
 * 
 * @param[in] Copy_enmPortNum        this attribute used to determine the port which you want to use
 *									 (EX. DIO_enmPortA)  , allowed only DIO_enmPortA ,DIO_enmPortB
 *									 DIO_enmPortC , DIO_enmPortD.
 *
 * @param[in] Copy_enmPinNum         refer to the pin which you want to use like (DIO_enumPin1) you
 *	 								 have only 8 pins in each port . the pins defined in DIO_enumPins_t
 *									 enum.
 * 									  
 * @param[in] Copy_enmLogicState    this attribute used for determine the pin is HIGH or LOW
 *									 (EX. DIO_enumLogicHigh) all Logic States pre-defined in 
 *									 DIO_enmLOutputPinState_t enum.
 *
 * @return enum contain Error State
 * 
 * @note							 you must use this function after configuring the pin as output.
/*----------------------------------------------------------------------------*/
DIO_enumError_t DIO_enumSetPin(DIO_enmPortOPTS Copy_enmPortNum,DIO_enumPins_t Copy_enmPinNum,DIO_enmLOutputPinState_t Copy_enmLogicState);



/*----------------------------------------------------------------------------*/
/**
 * @brief							 if you configured the A PORT as OUTPUT, you should use this function
 *									 to determine if this PORT pins is HIGH or LOW.
 * 
 * @param[in] Copy_enmPortNum        this attribute used to determine the port which you want to use
 *									 (EX. DIO_enmPortA)  , allowed only DIO_enmPortA ,DIO_enmPortB
 *									 DIO_enmPortC , DIO_enmPortD.
 *
 * 									  
 * @param[in] Copy_enmLogicState    this attribute used for determine the PORT pins is HIGH or LOW
 *									 (EX. DIO_enumLogicHigh) all Logic States pre-defined in 
 *									 DIO_enmLOutputPinState_t enum.
 *
 * @return enum contain Error State
 * 
 * @note							 you must use this function after configuring the Port.
/*----------------------------------------------------------------------------*/
DIO_enumError_t DIO_enumSetPort(DIO_enmPortOPTS Copy_enmPortNum,DIO_enmLOutputPinState_t Copy_enmLogicState);



/*----------------------------------------------------------------------------*/
/**
 * @brief							 if you configured the pin as Input, you should use this function
 *									 to read an pin.
 * 
 * @param[in] Copy_enmPortNum        this attribute used to determine the port which you want to use
 *									 (EX. DIO_enmPortA)  , allowed only DIO_enmPortA ,DIO_enmPortB
 *									 DIO_enmPortC , DIO_enmPortD.
 *
 * @param[in] Copy_enmPinNum         refer to the pin which you want to use like (DIO_enumPin1) you
 *	 								 have only 8 pins in each port . the pins defined in DIO_enumPins_t
 *									 enum.
 * 									  
 * @param[in/out] Add_Pu8PinValue    this attribute used for update the pin status.
 *
 * @return enum contain Error State
 * 
 * @note you must use this function after configuring the pin as input.
/*----------------------------------------------------------------------------*/
DIO_enumError_t DIO_enumGetState(DIO_enmPortOPTS Copy_enmPortNum,DIO_enumPins_t Copy_enmPinNum,uint8_t* Add_Pu8PinValue);






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
#endif /* DIO_H_ */
/******************************************************************************/
