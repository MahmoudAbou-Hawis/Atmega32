
#ifndef TIMER0_H_
#define TIMER0_H_

#include "Timer_CFG.h"
#include "../../00_LIB/Platform_Types.h"

/**
 * @file TMR0.h
 * @brief Header file that provides API declarations for the TMR0 (Timer 0) Driver.
 * 
 * This file defines the interface for the Timer 0 (TMR0) functionality of the
 * microcontroller. It provides function prototypes for initializing and configuring
 * the timer, setting tick times, starting the timer, and configuring PWM duty cycles.
 */



 /************************************************************************/
 /* ENUMS			                                        */
 /************************************************************************/
 typedef enum
 {
		/**
         *@brief returned if the function did it functionality correctly.
        */
	TMR0_enuOk,
	
	/**
	*@brief returned if you pass to function an Invalid Timer Mode.
	*/
	TMR0_enuInvalidMode ,


	/**
	*@brief returned if you pass to function an invalid time Interval.
	*/
	TMR0_enuInvalidTimerInterval,

		
	/**
	*@brief  returned if null pinter to function passed
	*/
	TMR0_enuNullPtr ,

	/**
	*@brief  returned if invalid prescaller passed to function
	*/
	TMR0_inValidPrescaller ,

	TMR0_eniInvalidIntSrc ,

	TMR0_enuNotValidDutyCycle ,

	TMR0_enuNotValidPWMType

 } TMR0_enuErrorStatus_t;

 #if (COMPILATION_MODE == POST_COMPILATION_MODE)
 typedef enum
 {
	enuNOT_PWM_TIMER ,
	enuPWM

 } TMR0_enmTimerModes_t;

 typedef enum
 {
	enuNO_PRESCALLER =1,

	enuPRESCALLER_8,

	enuPRESCALLER_64,

	enuPRESCALLER_256,

	enuPRESCALLER_1024	

 } TMR0_enmTimerPrescaller_t;


 typedef enum 
 {
	enuOVERFLOW ,

	enuCOMPARE_MATCH

 } TMR0_enmTimerIntSrc;


 typedef enum
 {
	_100MS_TICK_TIME = 100,
	_500MS_TICK_TIME = 500,
	_1S_TICK_TIME = 1000,
	_2S_TICK_TIME = 2000,
	_5S_TICK_TIME = 5000

 } TMR0_enuTicks;

 typedef enum 
 {
    TMR0_INVERTED , 
    TMR0_NON_INVERTED 
 } TMR0_enuPWMTypes;

 /************************************************************************/
 /* STRUCTS                                                              */
 /************************************************************************/
 typedef struct  
 {
     TMR0_enmTimerModes_t enmTimerMode ;

     TMR0_enmTimerPrescaller_t enmPrescaller;
	
     void (*CallBack)(void*);

     void * pFunctionParem;

 } TMR0Config_t;

 #endif
/* Function Prototypes */

#if COMPILATION_MODE == PRE_COMPILATION_MODE
/**
 * @brief Initialize the Timer 0 (TMR0) driver with pre-compiled settings.
 * 
 * This function performs the initialization of the TMR0 module with a predefined configuration.
 * It should be called before any other TMR0-related functions to ensure the timer is set up correctly.
 */
void TMR0_voidInitPreCompile(void);

#endif

#if COMPILATION_MODE == POST_COMPILATION_MODE
/**
 * @brief Initialize the Timer 0 (TMR0) driver with user-provided configuration.
 * 
 * @param Copy_strTimerConfig Pointer to a TMR0Config_t structure that contains the desired timer configuration.
 * @return TMR0_enuErrorStatus_t Error status of the initialization process.
 * 
 * This function allows for post-compilation initialization of the TMR0 module, enabling the user to
 * specify a configuration at run-time. 
 */
TMR0_enuErrorStatus_t TMR0_voidInitPostCompile(TMR0Config_t *Copy_strTimerConfig);

#endif
/**
 * @brief Set the desired tick time and interrupt source for Timer 0.
 * 
 * @param Copy_u8DesiredTime The desired tick time, which can be one of the predefined constants:
 *        100MS_TICK_TIME, 500MS_TICK_TIME, 1S_TICK_TIME, 2S_TICK_TIME, 5S_TICK_TIME.
 * @param Copy_u8InterruptSource The interrupt source, which can be either OVERFLOW or COMPARE_MATCH.
 * @return TMR0_enuErrorStatus_t Error status of the tick time setting process.
 * 
 * This function configures the tick time for the TMR0 module and specifies whether an interrupt should
 * be generated on an overflow or a compare match event. It returns an error status to indicate the success
 * or nature of any failure in setting the tick time.
 */
TMR0_enuErrorStatus_t TMR0_enuSetTickTimeMS(uint32_t Copy_u8DesiredTime, uint8_t Copy_u8InterruptSource);

/**
 * @brief Start Timer 0 to begin counting.
 * 
 * @return TMR0_enuErrorStatus_t Error status of the start process.
 * 
 * This function activates TMR0 to start counting based on the previously configured settings.
 * It returns an error status to indicate the success or nature of any failure in starting the timer.
 */
TMR0_enuErrorStatus_t TMR0_enuStart(void);

/**
 * @brief Set the duty cycle for Timer 0 in Fast PWM mode.
 * 
 * @param Copy_u8DutyCycle The desired duty cycle value.
 * @param Copy_u8PWMType The PWM type, which can be either INVERTED or NON_INVERTED.
 * @return TMR0_enuErrorStatus_t Error status of the duty cycle setting process.
 * 
 * This function configures the duty cycle for TMR0 when operating in Fast PWM mode. The duty cycle and PWM type
 * are specified by the user. It returns an error status to indicate success or the nature of any failure in setting
 * the duty cycle.
 */
 #if (COMPILATION_MODE == POST_COMPILATION_MODE) || (COMPILATION_MODE == PRE_COMPILATION_MODE && TIMER_MODE == PWM )
TMR0_enuErrorStatus_t TMR0_enuSetDutyCycleFastPWM(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType);

/**
 * @brief Set the duty cycle for Timer 0 in Phase Correct PWM mode.
 * 
 * @param Copy_u8DutyCycle The desired duty cycle value.
 * @param Copy_u8PWMType The PWM type, which can be either INVERTED or NON_INVERTED.
 * @return TMR0_enuErrorStatus_t Error status of the duty cycle setting process.
 * 
 * This function configures the duty cycle for TMR0 when operating in Phase Correct PWM mode. It allows
 * for finer control of the PWM waveform compared to Fast PWM mode. It returns an error status to indicate
 * the success or nature of any failure in setting the duty cycle.
 */
TMR0_enuErrorStatus_t TMR0_enuSetDutyCyclePhaseCorrection(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType);

#endif

#endif