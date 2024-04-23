/******************************************************************************/
/**
 * @file Timer.c
 * @brief Timer implementation for AVR drivers
 *
 * @par Project Name
 * AVR drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * Implements a versatile timer module supporting PWM, external event counting,
 * CTC mode, normal mode, and user-selectable timer choice.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/


/******************************************************************************/
/* INCLUDES 
/******************************************************************************/
#include "../../00_LIB/BIT_MATH.h"
#include "Timer.h"
#include "Reg.h"
#include "Timers_CFG.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/

/* prescaler is 1024 to decrease interrupt jitter*/
#define		 PRESCALER				1024

/**
*@brief this is the prescaler value to activate prescaler 1024
*/
#define		 PRESCALER_VALUE			5

/**
*@brief this is the prescaler value to activate prescaler 1024
*/
#define		 PRESCALER_VALUE_TIMER_2		7

/**
*@brief the capacity of the Timer 0 and TIMER 2 is 256 tick
*/
#define		 TIMER0_SIZE				256

/**
*@brief the capacity of the Timer 1 and TIMER 2 is 65536 tick
*/
#define		 TIMER1_SIZE				65536


/**
*@brief   Timer/Counter0 Overflow Interrupt Enable bit.
*/
#define		TOIE0					0

/**
*@brief   Timer/Counter1 Overflow Interrupt Enable bit.
*/
#define		TOIE1					2

/**
*@brief   Timer/Counter2 Overflow Interrupt Enable bit.
*/
#define		TOIE2					6

/**
*@brief   Timer/Counter0 compare match Interrupt Enable bit.
*/
#define		OCIE0					1

/**
*@brief used to enable CTC mode
*/
#define		CTC0					8

/**
*@brief   Timer/Counter2 compare match Interrupt Enable bit.
*/
#define		OCIE2					7

/**
*@brief   Timer/Counter1 compare match A Interrupt Enable bit.
*/
#define		OCIE1A					4

/**
*@brief   this refer to I bit in global interrupt.
*/
#define		SERG_INT_EN_BIT				7


#define		WGM11					1


#define		WGM10					0



#define		WGM13					4


#define		WGM12					3



#define		COM1A1					7


#define		COM1A0					6

/* Maximum value for 16-bit PWM */
#define		PWM_MAX_VALUE				1023  

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
/******************************************************************************/

/**
*@brief used to convert the micro-second to milli-second.
*/
#define		 SECONDTOMILLI(MICRO)			(MICRO * 1000)

#if TIMER0_ENABLE == ON
#define  MIN_TIMER Timer_enuTimer0
#elif TIMER1_ENABLE == ON
#define  MIN_TIMER Timer_enuTimer1
#elif TIMER2_ENABLE == ON
#define  MIN_TIMER Timer_enuTimer2
#endif

#if TIMER2_ENABLE == ON
#define  MAX_TIMER Timer_enuTimer2
#elif TIMER1_ENABLE == ON
#define  MAX_TIMER Timer_enuTimer1
#elif TIMER0_ENABLE == ON
#define  MAX_TIMER Timer_enuTimer0
#endif



/******************************************************************************/
/* PRIVATE ENUMS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE TYPES */
/******************************************************************************/

/**
*brief this contain the informations to update the timer each period of timer.
*/
typedef struct
{
    /**
    *@brief that variable contain the initial value for the timer and timer 
    *       updated with this value every TickCount. 
    */
    uint32_t u8Preload;

    /**
    *@brief contain number of interrupt to execute the Call Back
    *	    function.   
    */
    uint32_t u8TickCount;

    /**
    *@brief contain number of current interrupts .   
    */
    uint32_t u8CurrentTicksCount;

    /**
    *brief pointer to the function which will executed after finishing the time.
    */
     void (*ADD_CallBack)(void*);

     /**
     *brief this is a generic pointer to the parameters of the call back function.
     */
     void * vpFuncParam;

} stTimerInfo;

/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

/**
*@brief this is the time which will take to increase the timer by 1 .
*/
static double TickTime =  0;

/**
*@brief this array contain the all information timer need .
*/
static stTimerInfo astTimersInfo[NUMBER_OF_ACTIVITED_TIMERS] = {0};

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
#if TIMER0_ENABLE == ON
/**
* @brief Timer0 overflow interrupt service routine .
*/
void __vector_11(void) __attribute__((signal));

/**
* @brief timer0 compare interrupt service routine;
*/
void __vector_10(void) __attribute__((signal));
#endif

#if TIMER2_ENABLE == ON
/**
* @brief Timer2 overflow interrupt service routine .
*/
void __vector_5(void) __attribute__((signal));

/**
* @brief timer2 compare interrupt service routine;
*/
void __vector_4(void) __attribute__((signal));
#endif

#if TIMER1_ENABLE == ON

/**
* @brief Timer1 overflow interrupt service routine.
*/
void __vector_9(void) __attribute__((signal));

/**
* @brief timer1 compare interrupt service routine.
*/
void __vector_7(void) __attribute__((signal));
#endif
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/

#if TIMER0_ENABLE == ON || TIMER1_ENABLE == ON || TIMER2_ENABLE == ON
void Timer_vInit()
{
	/*Check if CPU frequency equal to zero */
	/* the CPU frequency should be larger than 0 */
	#if F_CPU == 0
		#error "F_CPU Can't be zero." 
	#endif 	
	/*calculate the time of tick in milli-seconds */
	TickTime = SECONDTOMILLI(((double)PRESCALER / F_CPU)) ;

	/*Enable global interrupt by set */
	SET_BIT(SREG,SERG_INT_EN_BIT);				
}


Timer_enuErrors_t Timer_enuSetTime(Timer_enuType_t Copy_enuTimerName,
				   Timer_enuWGMode Copy_enuMode,
				   Timer_enuTimeInterval Copy_enuTime)
{
	/* this is the error status for this function, initially it is OK , 
	   it is updated during the execution   */
	Timer_enuType_t RET_enuErrorStatus = Timer_enuOk;

	/*Check if the timer is not valid or disabled*/
	if(Copy_enuTimerName < MIN_TIMER || Copy_enuTimerName > MAX_TIMER)
	{
		RET_enuErrorStatus = Timer_enuInvalidTimer;
	}
	/*Check if the timer mode is valid mode*/
	else if(Copy_enuMode < Normal || Copy_enuMode > CTC)
	{
		RET_enuErrorStatus = Timer_enuInvalidMode;
	}
	/*check if the time interval is not invalid*/
	else if(Copy_enuTime < Timer_25_ms || Copy_enuTime > Timer_6_s)
	{
		RET_enuErrorStatus = Timer_enuInvalidTimerInterval;
	}
	else
	{

		switch(Copy_enuTimerName)
		{
			/*if the timer zero is on*/
			#if TIMER0_ENABLE == ON
			case Timer_enuTimer0:
				switch(Copy_enuMode)
				{
				/*in case you use compare match mode*/
				     case CTC:
				     /*it enable the output compare interrupt for timer 0*/
				     	SET_BIT(TIMSK, OCIE0);
					/*calculate the number of ticks need to achive the time needed*/
				        uint16_t NumberOfTick = ((Copy_enuTime * 25) / TickTime);
					/*This loop used for found number be divisible with the number of thicks*/
				        boolean NotFactor = TRUE;
				        for(uint8_t u32factor = TIMER0_SIZE -1 ; NotFactor ;u32factor--)
				        {
					        if(NumberOfTick % u32factor == 0)
					        {
						      OCR0 =  astTimersInfo[Copy_enuTimerName].u8Preload = u32factor;
						        NotFactor = FALSE;
					        }
				        }
					/*calculate how many numbers of interrupts need to execute the user function*/
					astTimersInfo[Copy_enuTimerName].u8TickCount = ((Copy_enuTime * 25) / TickTime) 
											/astTimersInfo[Copy_enuTimerName].u8Preload ;
					/*Enable the timer and put the prescaler as 1024*/
					SET_BIT_FIELD(TCCR0,PRESCALER_VALUE | CTC0,0);
				     break;

				     case Normal:
				     /*Enable the Timer0 overflow interrupt which in TIWSK register */
					SET_BIT(TIMSK, TOIE0);

					/*calculate the number of ticks need to execute the user function*/
					astTimersInfo[Copy_enuTimerName].u8TickCount = ((Copy_enuTime * 25) / TickTime) / TIMER0_SIZE;

					/*this is to calculate the preload (initial value for the counter). */
					 TCNT0 = astTimersInfo[Copy_enuTimerName].u8Preload =
						TIMER0_SIZE - ((uint16_t)((Copy_enuTime * 25) / TickTime) % TIMER0_SIZE);

					/*if the preload not equal to zero that mean we have reminder so tick added by one*/
						astTimersInfo[Copy_enuTimerName].u8TickCount +=
						(astTimersInfo[Copy_enuTimerName].u8Preload > 0)?1:0;
				     		SET_BIT_FIELD(TCCR0,PRESCALER_VALUE,0);

				     break;
				}

			break;
			#endif
			#if TIMER1_ENABLE == ON
			case Timer_enuTimer1:
				switch(Copy_enuMode)
				{
					case CTC:
					     SET_BIT(TIMSK, OCIE1A);
					     uint16_t NumberOfTick = ((Copy_enuTime * 25) / TickTime);
					     boolean NotFactor = TRUE;
					     OCR1A =  astTimersInfo[Copy_enuTimerName].u8Preload = NumberOfTick;
					     astTimersInfo[Copy_enuTimerName].u8TickCount = ((Copy_enuTime * 25) / TickTime)
					     /astTimersInfo[Copy_enuTimerName].u8Preload ;
					     SET_BIT_FIELD(TCCR1B,PRESCALER_VALUE | CTC0,0);
					break;

					case Normal:
						SET_BIT(TIMSK, TOIE1);
						SET_BIT_FIELD(TCCR1B,PRESCALER_VALUE,0);
						astTimersInfo[Copy_enuTimerName].u8TickCount = 
						 ((Copy_enuTime * 25) / TickTime) / TIMER1_SIZE;
						TCNT1 = astTimersInfo[Copy_enuTimerName].u8Preload =
						TIMER1_SIZE - ((uint16_t)((Copy_enuTime * 25) / TickTime) % TIMER1_SIZE);
							astTimersInfo[Copy_enuTimerName].u8TickCount +=
							(astTimersInfo[Copy_enuTimerName].u8Preload > 0)?1:0;
						break;
				}

			break;
			#endif
			#if TIMER2_ENABLE == ON
			case Timer_enuTimer2:
				switch(Copy_enuMode)
				{
					case CTC:
					SET_BIT(TIMSK, OCIE2);
					uint16_t NumberOfTick = ((Copy_enuTime * 25) / TickTime);
					boolean NotFactor = TRUE;

					for (uint16_t u32factor = TIMER0_SIZE - 1; NotFactor && u32factor > 1; u32factor--) {
						if (NumberOfTick % u32factor == 0) {
							OCR2 = astTimersInfo[Timer_enuTimer2].u8Preload = u32factor;
							NotFactor = FALSE;
						}
					}

					astTimersInfo[Timer_enuTimer2].u8TickCount = ((Copy_enuTime * 25) / TickTime) / astTimersInfo[Timer_enuTimer2].u8Preload;

					SET_BIT_FIELD(TCCR2, PRESCALER_VALUE_TIMER_2 | CTC0, 0);
					break;

					case Normal:
					SET_BIT(TIMSK, TOIE2);
					astTimersInfo[Copy_enuTimerName].u8TickCount =
					((Copy_enuTime * 25) / TickTime) / TIMER0_SIZE;
					TCNT2 = astTimersInfo[Copy_enuTimerName].u8Preload =
					TIMER0_SIZE - ((uint16_t)((Copy_enuTime * 25) / TickTime) % TIMER0_SIZE);
					astTimersInfo[Copy_enuTimerName].u8TickCount +=
					(astTimersInfo[Copy_enuTimerName].u8Preload > 0)?1:0;
					SET_BIT_FIELD(TCCR2,PRESCALER_VALUE_TIMER_2,0);
					break;
				}

			break;
			#endif
		} 
	}
	return RET_enuErrorStatus;
}


Timer_enuErrors_t Timer_enuSetCallBack(Timer_enuType_t Copy_enuTimerName, void (*ADD_CallBack)(void*), void * ADD_vpFuncParam)
{
	/*return the status of function is working right or not */
	Timer_enuErrors_t RET_enuErrorStatus = Timer_enuOk;		
		
	if(Copy_enuTimerName < MIN_TIMER || Copy_enuTimerName > MAX_TIMER)
	{
		RET_enuErrorStatus = Timer_enuInvalidTimer;
	}
	else if(ADD_CallBack == NULL)
	{
		/*return null function pointer passed to the function*/
		RET_enuErrorStatus = Timer_enuNullPtr;					
	}
	else
	{
		/**
		*@brief assign the  Call back and its params  for its interrupt.  
		*/
		astTimersInfo[Copy_enuTimerName].ADD_CallBack = ADD_CallBack;
		astTimersInfo[Copy_enuTimerName].vpFuncParam  = ADD_vpFuncParam;
	}
	return RET_enuErrorStatus;
}

#if TIMER1_PWM_ENABLE == ON
void Timer_enuPWM(uint8_t u8DutyCycle)
{	
	/**
	*  The following lines is to initialize the timer1
	*  to work as Fast PWM 10-bit with prescaler 1024
	*
	*/
	SET_BIT(TCCR1A,WGM10);
	SET_BIT(TCCR1A,WGM11);
	SET_BIT(TCCR1B,WGM12);
	CLR_BIT(TCCR1B,WGM13);
	SET_BIT(TCCR1A,COM1A1);
	CLR_BIT(TCCR1A,COM1A0);

	/**
	* This is the value which is put in output compare match
	* to generate the duty cycle.
	*/
	uint16_t u16OCR_Value = PWM_MAX_VALUE * (u8DutyCycle / (float)100);
	OCR1A =     u16OCR_Value;

	/*Put the prescaler and enable the timer to work*/
	SET_BIT_FIELD(TCCR1B,PRESCALER_VALUE,0);
}
#endif 
#if TIMER0_ENABLE == ON

void __vector_11(void)
{
	/*the current number of counts */
	uint32_t u8currentTicks =
		 ++astTimersInfo[Timer_enuTimer0].u8CurrentTicksCount;

	/*the target ticks*/
	uint32_t u8TargetTicks  = astTimersInfo[Timer_enuTimer0].u8TickCount;

	/* when we will arrive to the target execute the call back function*/
	if(u8currentTicks == u8TargetTicks)
	{
		/*it is a cycle you should put the initial value again*/
		TCNT0 = astTimersInfo[Timer_enuTimer0].u8Preload;

		/*restart the number of current Ticks counter */
		astTimersInfo[Timer_enuTimer0].u8CurrentTicksCount = 0;

		/*if the callback function is not equal to null execute it*/
		if(astTimersInfo[Timer_enuTimer0].ADD_CallBack != NULL)
		{
			astTimersInfo[Timer_enuTimer0].ADD_CallBack(
				astTimersInfo[Timer_enuTimer0].vpFuncParam);
		}
		else
		{
			/*No thing*/
		}
	}
	else
	{
		/*No thing*/
	}
}



void __vector_10(void)
{
	uint32_t u8currentTicks =
	++astTimersInfo[Timer_enuTimer0].u8CurrentTicksCount;
	uint32_t u8TargetTicks  = astTimersInfo[Timer_enuTimer0].u8TickCount;
	if(u8currentTicks == u8TargetTicks)
	{
		astTimersInfo[Timer_enuTimer0].u8CurrentTicksCount = 0;
		if(astTimersInfo[Timer_enuTimer0].ADD_CallBack != NULL)
		{
			astTimersInfo[Timer_enuTimer0].ADD_CallBack(
			astTimersInfo[Timer_enuTimer0].vpFuncParam);
		}
		else
		{
			/*No thing*/
		}
	}
	else
	{
		/*No thing*/
	}
}


#endif

#if TIMER2_ENABLE == ON

void __vector_5(void)
{
	uint32_t u8currentTicks =
	++astTimersInfo[Timer_enuTimer2].u8CurrentTicksCount;
	uint32_t u8TargetTicks  = astTimersInfo[Timer_enuTimer2].u8TickCount;
	if(u8currentTicks == u8TargetTicks)
	{
		TCNT2 = astTimersInfo[Timer_enuTimer2].u8Preload;
		astTimersInfo[Timer_enuTimer2].u8CurrentTicksCount = 0;
		if(astTimersInfo[Timer_enuTimer2].ADD_CallBack != NULL)
		{
			astTimersInfo[Timer_enuTimer2].ADD_CallBack(
			astTimersInfo[Timer_enuTimer2].vpFuncParam);
		}
		else
		{
			/*No thing*/
		}
	}
	else
	{
		/*No thing*/
	}
}

void __vector_4(void) {
	uint32_t u8currentTicks = ++astTimersInfo[Timer_enuTimer2].u8CurrentTicksCount;
	uint32_t u8TargetTicks = astTimersInfo[Timer_enuTimer2].u8TickCount;

	if (u8currentTicks == u8TargetTicks) {
		astTimersInfo[Timer_enuTimer2].u8CurrentTicksCount = 0;

		if (astTimersInfo[Timer_enuTimer2].ADD_CallBack != NULL) {
			astTimersInfo[Timer_enuTimer2].ADD_CallBack(astTimersInfo[Timer_enuTimer2].vpFuncParam);
		}
		// else: Do nothing if callback is not set
	}
	// else: Do nothing if target ticks not reached
}


#endif

#if TIMER1_ENABLE == ON
void __vector_9(void)
{
	uint32_t u8currentTicks =
	++astTimersInfo[Timer_enuTimer1].u8CurrentTicksCount;
	uint32_t u8TargetTicks  = astTimersInfo[Timer_enuTimer1].u8TickCount;
	if(u8currentTicks == u8TargetTicks)
	{
		TCNT1 = astTimersInfo[Timer_enuTimer1].u8Preload;
		astTimersInfo[Timer_enuTimer1].u8CurrentTicksCount = 0;
		if(astTimersInfo[Timer_enuTimer1].ADD_CallBack != NULL)
		{
			astTimersInfo[Timer_enuTimer1].ADD_CallBack(
				
				astTimersInfo[Timer_enuTimer1].vpFuncParam);
		}
		else
		{
			/*No thing*/
		}
	}
	else
	{
		/*No thing*/
	}
}


void __vector_7(void)
{
	uint32_t u8currentTicks =
	++astTimersInfo[Timer_enuTimer1].u8CurrentTicksCount;
	uint32_t u8TargetTicks  = astTimersInfo[Timer_enuTimer1].u8TickCount;
	if(u8currentTicks == u8TargetTicks)
	{
		astTimersInfo[Timer_enuTimer1].u8CurrentTicksCount = 0;
		if(astTimersInfo[Timer_enuTimer1].ADD_CallBack != NULL)
		{
			astTimersInfo[Timer_enuTimer1].ADD_CallBack(
			astTimersInfo[Timer_enuTimer1].vpFuncParam);
		}
		else
		{
			/*No thing*/
		}
	}
	else
	{
		/*No thing*/
	}
}


#endif


#endif

/******************************************************************************/

