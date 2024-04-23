/*
 * Timer1.h
 *
 * Created: 2/29/2024 9:17:49 PM
 *  Author: Mahmoud Sofar
 */ 

 #include "../../00_LIB/Platform_Types.h"
 
  typedef enum
 {
		/**
         *@brief returned if the function did it functionality correctly.
        */
	TMR1_enuOk,
	
	/**
	*@brief returned if you pass to function an Invalid Timer Mode.
	*/
	TMR1_enuInvalidMode ,


	/**
	*@brief returned if you pass to function an invalid time Interval.
	*/
	TMR1_enuInvalidTimerInterval,

		
	/**
	*@brief  returned if null pinter to function passed
	*/
	TMR1_enuNullPtr ,

	/**
	*@brief  returned if invalid prescaller passed to function
	*/
	TMR1_inValidPrescaller ,

	TMR1_eniInvalidIntSrc ,

	TMR1_enuNotValidDutyCycle ,

	TMR1_enuNotValidPWMType ,

	TMR1_enuError

 } TMR1_enuErrorStatus_t;




typedef enum
  {
	enuNO_PRESCALLER =1,

	enuPRESCALLER_8,

	enuPRESCALLER_64,

	enuPRESCALLER_256,

	enuPRESCALLER_1024

} TMR1_enmTimerPrescaller_t;



typedef enum
{
	enuOVERFLOW ,

	enuCOMPARE_MATCH

} TMR1_enmTimerIntSrc;



typedef enum
{
	TMR1_INVERTED ,
	TMR1_NON_INVERTED
} TMR1_enuPWMTypes;

typedef enum
{
	enuNOT_PWM_TIMER ,
	enuPWM

} TMR1_enmTimerModes_t;



typedef struct
{
	TMR1_enmTimerModes_t enmTimerMode ;

	TMR1_enmTimerPrescaller_t enmPrescaller;
	
	uint64_t CLK;	
	
	void (*CallBack)(void*);

	void * pFunctionParem;

} TMR1Config_t;



TMR1_enuErrorStatus_t TMR1_voidInit(TMR1Config_t *Copy_strTimerConfig);


TMR1_enuErrorStatus_t TMR1_enuSetTickTimeMS(uint32_t Copy_u8DesiredTime, uint8_t Copy_u8InterruptSource);


TMR1_enuErrorStatus_t TMR1_enuStart(void);


TMR1_enuErrorStatus_t TMR1_enuSetDutyCycleFastPWM(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType);


TMR1_enuErrorStatus_t TMR1_enuSetDutyCyclePhaseCorrection(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType);
