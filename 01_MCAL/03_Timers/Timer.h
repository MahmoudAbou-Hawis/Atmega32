/*******************************************************************************/
/**
 * @file Timer.h
 * @brief Header file for timer functionality
 *
 * @par Project Name
 * A
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This file defines the functions, macros, and data structures used for
 * configuring and managing timers within the project.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/
 
/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_
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
#include "Timers_CFG.h"
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
*@brief this enumeration contain all timers you can choose in atmega32. 
*/
typedef enum
{
    #if TIMER0_ENABLE == ON
    /*
    *@brief this present the timer 0 which is 8-bit timer.
    */
    Timer_enuTimer0 ,
    #endif

    #if TIMER1_ENABLE == ON
    /*
    *@brief this present the timer 1 which is 8-bit timer.
    */
    Timer_enuTimer1 ,
     #endif

     #if TIMER2_ENABLE == ON
    /*
    *@brief this present the timer which is 16-bit timer.
    */
    Timer_enuTimer2
    #endif

} Timer_enuType_t;



/*
*@brief these are all Timer errors which may occur when using APIs in wrong way.
*
*/
typedef enum
{
	/**
         *@brief returned if the function did it functionality correctly.
        */
	Timer_enuOk,
	
	/**
         *@brief returned if you pass to function an Invalid Timer or disabled Timer.
	 * available Timers :-
	 *	1- Timer_enuTimer0
	 *	2- Timer_enuTimer1
	 *	3- Timer_enuTimer2
        */
	Timer_enuInvalidTimer ,	

	/**
	*@brief returned if you pass to function an Invalid Timer Mode.
	*/
	Timer_enuInvalidMode ,


	/**
	*@brief returned if you pass to function an invalid time Interval.
	*/
	Timer_enuInvalidTimerInterval,

		
	/**
	*@brief  returned if null pinter to function passed
	*/
	Timer_enuNullPtr
} Timer_enuErrors_t;


/**
* @brief represent the Waveform Generation Mode.
*/
typedef enum 
{
	/**
	*@brief represent the normal mode in timer , Over flow mode.
	*/
	Normal ,

	/**
	 * @brief Configures the timer to operate in Clear Timer on Compare (CTC) mode,
	 *        where the counter resets automatically when it matches the value in
	 *        the Output Compare Register (OCR). This mode is useful for generating
	 *        precise timing intervals or periodic events.
	 */
	CTC

} Timer_enuWGMode;

/**
*@brief all intervals of time you can use.
*/
typedef enum
{
	Timer_25_ms = 1, 
	Timer_50_ms, 
	Timer_75_ms, 
	Timer_100_ms, 
	Timer_125_ms, 
	Timer_150_ms, 
	Timer_175_ms, 
	Timer_200_ms,
	Timer_225_ms, 
	Timer_250_ms, 
	Timer_275_ms, 
	Timer_300_ms, 
	Timer_325_ms, 
	Timer_350_ms, 
	Timer_375_ms,
	Timer_400_ms, 
	Timer_425_ms, 
	Timer_450_ms, 
	Timer_475_ms, 
	Timer_500_ms, 
	Timer_525_ms, 
	Timer_550_ms,
	Timer_575_ms, 
	Timer_600_ms, 
	Timer_625_ms, 
	Timer_650_ms, 
	Timer_675_ms, 
	Timer_700_ms, 
	Timer_725_ms,
	Timer_750_ms, 
	Timer_775_ms, 
	Timer_800_ms, 
	Timer_825_ms, 
	Timer_850_ms, 
	Timer_875_ms, 
	Timer_900_ms,
	Timer_925_ms, 
	Timer_950_ms,
	Timer_975_ms,
	Timer_1_s,
	Timer_2_s = 80,
	Timer_3_s = 120,
	Timer_4_s = 160,
	Timer_5_s = 200,
	Timer_6_s = 240,

} Timer_enuTimeInterval;

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


/*
* @brief used to initialize configured timer .
*
*
*@note      you should Enable timers in configurations file to enable it as an option.
*/
void Timer_vInit();


/**
* @brief			  This function is employed to establish the time period 
*				  for the selected timer.
*
* @param[in]	Copy_enuTimerName represent timer name which you want to work
*			         ,you must enable it first.
*
*
* @param[in]	Copy_enuMode	  represent the the type of  Waveform Generation Mode.
*				  it should be :
*				  - CTC		or
*				  - Normal
*
*
* @param[in]	Copy_enuTime	  Indicate the timing of the periodic interrupt.
*
*
*@return Timer error status
*
*
*@note				  you should enable the timer first then initialize it 
*				  , and call this function.
*/
Timer_enuErrors_t Timer_enuSetTime(Timer_enuType_t Copy_enuTimerName,
				   Timer_enuWGMode Copy_enuMode,
				   Timer_enuTimeInterval Copy_enuTime);


/**
* @brief			    this function used to set callback function 
*				    which will called when the interrupt occur.
*
*
* @param[in] Copy_enuTimerName      Determines the Timer name of the interrupt.
*				    When triggered, this interrupt will
*   			      	    call the specified callback.
*
*
* @param[in] ADD_CallBack	    Contain the address of the function 
*				    which called when interrupt occur.
*
*
*
* @param[in] ADD_vpFuncParam	    generic pointer to the any data type.
*				    which you can use.
*
*
*/
Timer_enuErrors_t Timer_enuSetCallBack(Timer_enuType_t Copy_enuTimerName,
				       void (*ADD_CallBack)(void*),
				       void * ADD_vpFuncParam);



/**
*@brief this function used to set PWM , it generate the PWM in OC1A – (Port D, Bit 5).
*
*
*@param[in]	u8DutyCycle		use to enter the Duty Cycle in percentage  
*
*@return	no thing
*
*
*@note		you can use it without Enable timer 1.
*/#if TIMER1_PWM_ENABLE == ON
void Timer_enuPWM(uint8_t u8DutyCycle);
#endif

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
#endif /* TIMER_H_ */
/******************************************************************************/
