/*
 * Timer0.c
 *
 * Created: 2/5/2024 6:55:29 PM
 *  Author: Mahmoud Sofar
 */ 
 #include "../../00_LIB/Platform_Types.h"
 #include "Timer0.h"
 #include "../03_Timers/Reg.h"
 #include "../../00_LIB/BIT_MATH.h"



/**
*@brief   this refer to I bit in global interrupt.
*/
#define		SERG_INT_EN_BIT				7
#define		CS02				        2
#define		CS01				        1
#define		CS00					0


#define		WGM00					 6
#define		COM01					 5
#define		COM00					 4
#define		WGM01					 3


/**
*@brief   Timer/Counter0 Overflow Interrupt Enable bit.
*/
#define		TOIE0					0


/**
*@brief   Timer/Counter0 compare match Interrupt Enable bit.
*/
#define		OCIE0					1

/**
*@brief used to convert the micro-second to milli-second.
*/
#define		 SECONDTOMILLI(MICRO)			(MICRO * 1000)


#define		 SET_PRESCALLER(_PRESCALLER)		TCCR0 |= _PRESCALLER

#define		CFG_IS_CURRENT_MODE(_MODE)		(PRESCALLER == _MODE)
/**
*@brief this is the time which will take to increase the timer by 1 .
*/
static double TickTime =  0;
 #if COMPILATION_MODE == POST_COMPILATION_MODE
 TMR0Config_t *strTimerConfig = NULL;
 #endif
/**
*@brief that variable contain the initial value for the timer and timer 
*       updated with this value every TickCount. 
*/
static uint32_t u8Preload;

/**
*@brief contain number of interrupt to execute the Call Back
*	    function.   
*/
static uint32_t u32TickCount;

/**
*@brief contain number of current interrupts .   
*/
static uint32_t u8CurrentTicksCount;


/**
* @brief Timer0 overflow interrupt service routine .
*/
void __vector_11(void) __attribute__((signal));

/**
* @brief timer0 compare interrupt service routine;
*/
void __vector_10(void) __attribute__((signal));






#if COMPILATION_MODE == PRE_COMPILATION_MODE
	void TMR0_voidInitPreCompile(void)
	{
			#if F_CPU == 0
			#error "F_CPU Can't be zero."
			#endif
			/*calculate the time of tick in milli-seconds */
			TickTime = SECONDTOMILLI(((double)PRESCALLER / F_CPU)) ;

			/*Enable global interrupt by set */
			SET_BIT(SREG,SERG_INT_EN_BIT);
	}
#endif


#if COMPILATION_MODE == POST_COMPILATION_MODE


TMR0_enuErrorStatus_t TMR0_voidInitPostCompile(TMR0Config_t *Copy_strTimerConfig)
{
	#if F_CPU == 0
	#error "F_CPU Can't be zero."
	#endif
	strTimerConfig = Copy_strTimerConfig;
	TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
	if(Copy_strTimerConfig == NULL)
	{
		RET_enuErrorStatus = TMR0_enuNullPtr;
	}
	else if(Copy_strTimerConfig->enmPrescaller > enuPRESCALLER_1024 ||
		Copy_strTimerConfig->enmPrescaller < enuNO_PRESCALLER)
	{
		RET_enuErrorStatus = TMR0_inValidPrescaller;
	}
	else if(Copy_strTimerConfig->enmTimerMode > PWM || Copy_strTimerConfig->enmTimerMode < NOT_PWM_TIMER)
	{
		RET_enuErrorStatus = TMR0_enuInvalidMode;
	}
	else
	{
		uint16_t u8Prescaller = (Copy_strTimerConfig->enmPrescaller == enuNO_PRESCALLER) * 1  +
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_8)  * 8  + 
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_64) * 64 + 
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_256)* 256+
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_1024) *1024;

		TickTime = SECONDTOMILLI(((double)u8Prescaller / F_CPU)) ;

		/*Enable global interrupt by set */
		SET_BIT(SREG,SERG_INT_EN_BIT);
	}
	return RET_enuErrorStatus;
}

#endif


TMR0_enuErrorStatus_t TMR0_enuSetTickTimeMS(uint32_t Copy_u8DesiredTime,
					    uint8_t Copy_u8InterruptSource)
{
	TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
	if(Copy_u8DesiredTime != _100MS_TICK_TIME && Copy_u8DesiredTime != _500MS_TICK_TIME 
	&& Copy_u8DesiredTime != _1S_TICK_TIME && Copy_u8DesiredTime != _2S_TICK_TIME 
	&& Copy_u8DesiredTime != _5S_TICK_TIME )
	{
		RET_enuErrorStatus = TMR0_eniInvalidIntSrc;
	}
	else if(Copy_u8InterruptSource != enuOVERFLOW && Copy_u8InterruptSource != enuCOMPARE_MATCH)
	{

		RET_enuErrorStatus = TMR0_enuInvalidTimerInterval;
	}
	else if(strTimerConfig->enmTimerMode == PWM)
	{
		RET_enuErrorStatus = TMR0_enuInvalidMode;
	}
	else
	{
		TCCR0 &= 0xF8;	
		uint16_t NumberOfTick ;	
		switch(Copy_u8InterruptSource)
		{
			case enuOVERFLOW:
				u32TickCount =Copy_u8DesiredTime / (TickTime * 255);
				TCNT0 = u8Preload = 255 - ((Copy_u8DesiredTime % ((uint16_t)(255 * TickTime))));
				u32TickCount += (u8Preload > 0)?1:0;
				uint8_t IntRegTemp = TIMSK;
				SET_BIT(IntRegTemp,TOIE0);
				CLR_BIT(IntRegTemp,OCIE0);
				TIMSK = IntRegTemp;
				break;
				case enuCOMPARE_MATCH:
				 NumberOfTick = (uint16_t)(Copy_u8DesiredTime / TickTime);
				boolean u8NotFactor = TRUE;
				for(uint8_t u32factor = 255 ; u8NotFactor ;u32factor--)
				{
					if(NumberOfTick % u32factor == 0)
					{
						OCR0 =  u8Preload = u32factor;
						u8NotFactor = 0;
					}
				}
				u32TickCount =  NumberOfTick / u8Preload;
				uint8_t IntRegTemp2 = TIMSK;
				CLR_BIT(IntRegTemp2,TOIE0);
				SET_BIT(IntRegTemp2,OCIE0);
				TIMSK = IntRegTemp2;
				break;
		}
	}
	return RET_enuErrorStatus;
}

TMR0_enuErrorStatus_t TMR0_enuStart(void)
{
	#if COMPILATION_MODE == PRE_COMPILATION_MODE
		uint8_t u8Prescaler = 
			CFG_IS_CURRENT_MODE(PRESCALLER_1024) * 5
			+ CFG_IS_CURRENT_MODE(PRESCALLER_256) * 4
			+ CFG_IS_CURRENT_MODE(PRESCALLER_64) * 3
			+ CFG_IS_CURRENT_MODE(PRESCALLER_8) * 2
			+ CFG_IS_CURRENT_MODE(NO_PRESCALLER) * 1;
	SET_PRESCALLER(u8Prescaler);

	#elif COMPILATION_MODE == POST_COMPILATION_MODE
		uint8_t u8Temp = TCCR0;
		CLR_BIT(u8Temp,CS00);
		CLR_BIT(u8Temp,CS01);
		CLR_BIT(u8Temp,CS02); 
		u8Temp |= strTimerConfig->enmPrescaller;
		TCCR0 = u8Temp;
	#else	
		#error COMPILATION_MODE Is configure in wrong way.

	#endif

}


TMR0_enuErrorStatus_t TMR0_enuSetDutyCycleFastPWM(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType)
{	
	TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
	if(Copy_u8DutyCycle <= 0 || Copy_u8DutyCycle > 100)
	{
		RET_enuErrorStatus = TMR0_enuNotValidDutyCycle;
	}
	else if(Copy_u8PWMType != TMR0_INVERTED && Copy_u8PWMType != TMR0_NON_INVERTED)
	{
		RET_enuErrorStatus = TMR0_enuNotValidPWMType;
	}
	else
	{
		uint8_t u8Temp = TCCR0;
		SET_BIT(u8Temp,WGM00);
		SET_BIT(u8Temp,WGM01);
		TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
		if(Copy_u8PWMType == TMR0_INVERTED)
		{
			SET_BIT(u8Temp,COM00);
			SET_BIT(u8Temp,COM01);
		}
		else
		{
			CLR_BIT(u8Temp,COM00);
			SET_BIT(u8Temp,COM01);
		}
		TCCR0 = u8Temp;
		uint16_t duty = (((uint32_t)Copy_u8DutyCycle * (uint32_t)255) / 100);
		OCR0 = duty;
	}
	return RET_enuErrorStatus;
}

TMR0_enuErrorStatus_t TMR0_enuSetDutyCyclePhaseCorrection(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType)
{
	TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
	if(Copy_u8DutyCycle <= 0 || Copy_u8DutyCycle > 100)
	{
		RET_enuErrorStatus = TMR0_enuNotValidDutyCycle;
	}
	else if(Copy_u8PWMType != TMR0_INVERTED && Copy_u8PWMType != TMR0_NON_INVERTED)
	{
		RET_enuErrorStatus = TMR0_enuNotValidPWMType;
	}
	else
	{
		uint8_t u8Temp = TCCR0;
		CLR_BIT(u8Temp,WGM00);
		SET_BIT(u8Temp,WGM01);
		TMR0_enuErrorStatus_t RET_enuErrorStatus = TMR0_enuOk;
		if(Copy_u8PWMType == TMR0_INVERTED)
		{
			SET_BIT(u8Temp,COM00);
			SET_BIT(u8Temp,COM01);
		}
		else
		{
			CLR_BIT(u8Temp,COM00);
			SET_BIT(u8Temp,COM01);
		}
		TCCR0 = u8Temp;
		uint16_t duty = (((uint32_t)Copy_u8DutyCycle * (uint32_t)255) / 100);
		OCR0 = duty;
	}
	return RET_enuErrorStatus;
}

void __vector_11(void)
{
	/*the current number of counts */
	uint32_t u8currentTicks = ++u8CurrentTicksCount;

	/*the target ticks*/
	uint32_t u32TargetTicks  = u32TickCount;

	/* when we will arrive to the target execute the call back function*/
	if(u8currentTicks == u32TargetTicks)
	{
		/*it is a cycle you should put the initial value again*/
		TCNT0 = u8Preload;

		/*restart the number of current Ticks counter */
		u8CurrentTicksCount = 0;
		 #if COMPILATION_MODE == POST_COMPILATION_MODE
		/*if the callback function is not equal to null execute it*/
		if( strTimerConfig->CallBack != NULL)
		{
			strTimerConfig->CallBack(
			strTimerConfig->pFunctionParem);
		}
		else
		{
			/*No thing*/
		}
		#endif
	}
	else
	{
		/*No thing*/
	}
}



void __vector_10(void)
{
	/*the current number of counts */
	uint32_t u8currentTicks = ++u8CurrentTicksCount;

	/*the target ticks*/
	uint32_t u32TargetTicks  = u32TickCount;

	/* when we will arrive to the target execute the call back function*/
	if(u8currentTicks == u32TargetTicks)
	{
		/*restart the number of current Ticks counter */
		u8CurrentTicksCount = 0;
		#if COMPILATION_MODE == POST_COMPILATION_MODE
		/*if the callback function is not equal to null execute it*/
		if( strTimerConfig->CallBack != NULL)
		{
			strTimerConfig->CallBack(
			strTimerConfig->pFunctionParem);
		}
		else
		{
			/*No thing*/
		}
		#endif
	}
	else
	{
		/*No thing*/
	}
}

