/*
 * Timer1.c
 *
 * Created: 2/29/2024 9:07:37 PM
 *  Author: Mahmoud Sofar
 */ 

 #include "Timer1.h"
 #include "Timer1_ex.h"


static TMR1Config_t * CurrentConf = NULL;

static uint64_t CLKAfterPrescaller = 0;

static uint16_t PreLoad = 0;

static uint8_t NumberOfInterrupts = 0;

static uint8_t OverFlowCnts = 0;


static uint64_t  NumberOfTicks = 0;

/**
* @brief Timer1 overflow interrupt service routine.
*/
void __vector_9(void) __attribute__((signal));

/**
* @brief timer1 compare interrupt service routine.
*/
void __vector_7(void) __attribute__((signal));

 double ValueInComp  = 0;
 double prestage =  100;

TMR1_enuErrorStatus_t TMR1_voidInit(TMR1Config_t *Copy_strTimerConfig)
{
	TMR1_enuErrorStatus_t RET_enuErrorStatus = TMR1_enuOk;
	if(Copy_strTimerConfig == NULL)
	{
		RET_enuErrorStatus = TMR1_enuNullPtr;
	}
	else
	{
		CurrentConf = Copy_strTimerConfig;
		uint16_t u16Prescaller = (Copy_strTimerConfig->enmPrescaller == enuNO_PRESCALLER) * 1  +
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_8)  * 8  +
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_64) * 64 +
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_256)* 256+
					(Copy_strTimerConfig->enmPrescaller == enuPRESCALLER_1024) * 1024;

		CLKAfterPrescaller = Copy_strTimerConfig->CLK / u16Prescaller;
		SREG |= (1 << SERG_INT_EN_BIT);
	}
	return RET_enuErrorStatus;
}


TMR1_enuErrorStatus_t TMR1_enuSetTickTimeMS(uint32_t Copy_u8DesiredTime, uint8_t Copy_u8InterruptSource)
{
	TMR1_enuErrorStatus_t RET_enuErrorStatus = TMR1_enuOk;
	if(Copy_u8DesiredTime <  0 ||CurrentConf == NULL || CurrentConf->enmTimerMode == enuPWM)
	{
		RET_enuErrorStatus = TMR1_enuError;
	}
	else if(Copy_u8InterruptSource > enuCOMPARE_MATCH || Copy_u8InterruptSource < enuOVERFLOW)
	{
		RET_enuErrorStatus = TMR1_enuError;
	}
	else
	{
		uint8_t EnabledEnterrupt = TIMSK;
		EnabledEnterrupt &= 195;
		double timePerTick  = 0;
		switch(Copy_u8InterruptSource)
		{
			case enuOVERFLOW:
				EnabledEnterrupt |= (1 <<  TOIE1);
				timePerTick = ((double)1000 / CLKAfterPrescaller);
				NumberOfTicks = ((double)Copy_u8DesiredTime / timePerTick);
				PreLoad = (65535 - (NumberOfTicks % 65535));
				PreLoad = ((PreLoad == 65535) ? 0 : PreLoad);
				NumberOfInterrupts = NumberOfTicks / 65535;
				NumberOfInterrupts += ((PreLoad > 0)? 1 :0);
				TCNT1 = PreLoad;
				break;
			case enuCOMPARE_MATCH:
				EnabledEnterrupt |= (1 <<  OCIE1A);
				timePerTick = ((double)1000 / CLKAfterPrescaller);
				NumberOfTicks = ((double)Copy_u8DesiredTime / timePerTick);
				boolean Acceptable = FALSE;
				uint64_t Value = 0;
				for(uint64_t Factor = 1 ; Factor * Factor <= NumberOfTicks && !Acceptable ; Factor++)
				{
					if(NumberOfInterrupts % Factor == 0 && NumberOfTicks/ Factor <= 65535)
					{
						Acceptable = TRUE;
						Value = NumberOfTicks/ Factor;
					}
				}
				OCR1A = Value;
				NumberOfInterrupts = NumberOfTicks / Value; 
				break;
			default:
				break;
		}
		TIMSK = EnabledEnterrupt;
	}
	return RET_enuErrorStatus;
}



TMR1_enuErrorStatus_t TMR1_enuStart(void)
{
	TMR1_enuErrorStatus_t  RET_enuErrorStatus = TMR1_enuOk;
	if(CurrentConf->enmPrescaller > enuPRESCALLER_1024 || CurrentConf->enmPrescaller < enuNO_PRESCALLER || CurrentConf == NULL)
	{
		RET_enuErrorStatus = TMR1_enuError;
	}
	else
	{
		uint8_t u8Prescaller = TCCR1B;
		u8Prescaller &= ~(7);
		u8Prescaller |= CurrentConf->enmPrescaller;
		TCCR1B = u8Prescaller;
	}
	return RET_enuErrorStatus;
}


TMR1_enuErrorStatus_t TMR1_enuSetDutyCycleFastPWM(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType)
{
	TMR1_enuErrorStatus_t RET_enuErrorStatus = TMR1_enuOk;
	if(CurrentConf->enmTimerMode == enuNOT_PWM_TIMER)
	{
		RET_enuErrorStatus = TMR1_enuError;
	}
	else if(Copy_u8DutyCycle > 100)
	{
		RET_enuErrorStatus = TMR1_enuNotValidDutyCycle;
	}
	else if(Copy_u8PWMType != TMR1_INVERTED && Copy_u8PWMType != TMR1_NON_INVERTED)
	{
		RET_enuErrorStatus = TMR1_enuNotValidPWMType;
	}
	else
	{
		TCCR1A |= (1 << WGM10 ) | (1 << WGM11);
		TCCR1B |= (1 << WGM12);
		switch(Copy_u8PWMType)
		{
			case TMR1_INVERTED :
				TCCR1A |= (1<< COM1A1) | (1 << COM1A0);
				break;
			case  TMR1_NON_INVERTED :
				TCCR1A &= ~(1 << COM1A0);
				TCCR1A |= (1 << COM1A1);
				break;
			default: 
				break;
		}
		 ValueInComp = ((Copy_u8DutyCycle * 1023UL) / prestage);
		OCR1A = ((uint16_t)ValueInComp);
		TCNT1 = 0;
	}
	return RET_enuErrorStatus;
}


TMR1_enuErrorStatus_t TMR1_enuSetDutyCyclePhaseCorrection(uint8_t Copy_u8DutyCycle, uint8_t Copy_u8PWMType)
{
	TMR1_enuErrorStatus_t RET_enuErrorStatus = TMR1_enuOk;
	if(CurrentConf->enmTimerMode == enuNOT_PWM_TIMER)
	{
		RET_enuErrorStatus = TMR1_enuError;
	}
	else if(Copy_u8DutyCycle > 100)
	{
		RET_enuErrorStatus = TMR1_enuNotValidDutyCycle;
	}
	else if(Copy_u8PWMType != TMR1_INVERTED && Copy_u8PWMType != TMR1_NON_INVERTED)
	{
		RET_enuErrorStatus = TMR1_enuNotValidPWMType;
	}
	else
	{
		TCCR1A |= (1 << WGM10 ) | (1 << WGM11);
		TCCR1A &= ~(1 << WGM12);
		TCCR1A &= ~(1 << WGM13);
		switch(Copy_u8PWMType)
		{
			case TMR1_INVERTED :
			TCCR1A |= (1<< COM1A1) | (1 << COM1A0);
			break;
			case  TMR1_NON_INVERTED :
			TCCR1A &= ~(1 << COM1A0);
			TCCR1A |= (1 << COM1A1);
			break;
			default:
			break;
		}
		 ValueInComp = ((Copy_u8DutyCycle * 1023UL) / prestage);
		 OCR1A = ((uint16_t)ValueInComp);
		 TCNT1 = 0;
	}
	return RET_enuErrorStatus;
}




void __vector_9(void) 
{
	OverFlowCnts++;
	if(NumberOfInterrupts == OverFlowCnts)
	{
		OverFlowCnts = 0;
		TCNT1 = PreLoad;
		if(CurrentConf->CallBack == NULL)
		{
			
		}
		else
		{
			CurrentConf->CallBack(CurrentConf->pFunctionParem);
		}
	}
}

void __vector_7(void)
{
	OverFlowCnts++;
	if(NumberOfInterrupts == OverFlowCnts)
	{
		OverFlowCnts = 0;
		TCNT1 = 0;
		if(CurrentConf->CallBack == NULL)
		{
			
		}
		else
		{
			CurrentConf->CallBack(CurrentConf->pFunctionParem);
		}
	}

}