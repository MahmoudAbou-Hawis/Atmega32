/*
 * ADC.c
 *
 * Created: 3/12/2024 9:21:02 PM
 *  Author: Mahmoud Sofar
 */ 

 #include "../../00_LIB/Platform_Types.h"
 #include "ADC.h"

 #define ADC_BASE		0x24

 #define ADC			((tstADCRegistrs *)ADC_BASE)

 #define	SREG			*((volatile uint8_t *)0x5F)


 #define	SERG_INT_EN_BIT		7

 /* ADCSRA */
 #define ADEN    7
 #define ADSC    6
 #define ADATE   5
 #define ADIF    4
 #define ADIE    3
 #define ADPS2   2
 #define ADPS1   1
 #define ADPS0   0

 /* ADMUX */
 #define REFS1   7
 #define REFS0   6
 #define ADLAR   5
 #define MUX4    4
 #define MUX3    3
 #define MUX2    2
 #define MUX1    1
 #define MUX0    0

 typedef struct
{
    /** @brief The identifier of the ADCL register in ADC
     *
     */
    volatile uint8_t u8Adcl;

    /** @brief The identifier of the u8Adch register in ADC
     *
     */
    volatile uint8_t u8Adch;

    /** @brief The identifier of the Adcsra register in ADC
     *
     */
    volatile uint8_t u8Adcsra;

    /** @brief The identifier of the u8Admux register in ADC
     *
     */
    volatile uint8_t u8Admux;

} tstADCRegistrs;


/**
* @brief ADC Conversion Complete Interrupt service routine. 
*/
void __vector_16(void) __attribute__((signal));

static void (*Callback)(void) = NULL;


static uint8_t Idx_Conversion = 0;

static ADC_strChain_t * strChain = NULL;

boolean ADC_Working = FALSE;

ADC_enuErrorStatus_t ADC_vInit(ADC_tstInitConfig* pstADCInit)
{
	ADC_enuErrorStatus_t RET_ErrorStatus  = ADC_OK;
	if(pstADCInit == NULL)
	{
		RET_ErrorStatus =  ADC_NULL_PTR_PASSED;
	}
	else if(pstADCInit->enmInterruptType != ADC_INTERRUPT_EN && pstADCInit->enmInterruptType != ADC_INTERRUPT_NONE)
	{
		
		RET_ErrorStatus = ADC_NOT_OK;
	}
	else if(pstADCInit->enmPrescalerSelections != ADC_Division_Factor_8 &&
	pstADCInit->enmPrescalerSelections !=  ADC_Division_Factor_2 &&
	 pstADCInit->enmPrescalerSelections != ADC_Division_Factor_4 &&
	 pstADCInit->enmPrescalerSelections != ADC_Division_Factor_16 &&
	 pstADCInit->enmPrescalerSelections != ADC_Division_Factor_32 &&
	 pstADCInit->enmPrescalerSelections != ADC_Division_Factor_64 &&
	 pstADCInit->enmPrescalerSelections != ADC_Division_Factor_128)
	{
		RET_ErrorStatus = ADC_NOT_OK;
	}
	else if(pstADCInit->enmVoltageReference != ADC_Avcc &&
	 pstADCInit->enmVoltageReference != ADC_INTERNAL_1V &&
	 pstADCInit->enmVoltageReference != ADC_ARED )
	 {
		RET_ErrorStatus = ADC_NOT_OK;
	 }
	else
	{
		uint8_t AdmuxTemp = ADC->u8Admux; ;
		AdmuxTemp &= ~(3 << REFS0);
		AdmuxTemp |= (pstADCInit->enmVoltageReference << REFS0);
		ADC->u8Admux = AdmuxTemp;
		uint8_t ADCSRA_temp = ADC->u8Adcsra;
		ADCSRA_temp &= ~(15);
		ADCSRA_temp |= pstADCInit->enmPrescalerSelections ;
		ADCSRA_temp |= (pstADCInit->enmInterruptType << ADIE);
		ADC->u8Adcsra = ADCSRA_temp;
		SREG |= (1 << SERG_INT_EN_BIT);

	}
	return RET_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuStartConversionSync(ADC_tenmAnolagChannals Copy_u8Channel_Id
						  ,uint16_t * Add_pu16ADCRead)
{
            ADC_enuErrorStatus_t RET_ErrorStatus  = ADC_OK;

	    if(Copy_u8Channel_Id !=ADC0 &&Copy_u8Channel_Id !=ADC1&&Copy_u8Channel_Id !=ADC2
	      &&Copy_u8Channel_Id !=ADC3&&Copy_u8Channel_Id !=ADC4&&Copy_u8Channel_Id !=ADC5 
	      && Copy_u8Channel_Id != ADC6 && Copy_u8Channel_Id != ADC7)
	      {
		RET_ErrorStatus = ADC_NOT_OK;
	      }
	     else
	     {
		    ADC->u8Admux &= 0xF0;
		    ADC->u8Admux |= Copy_u8Channel_Id;
		    ADC->u8Adcsra |= (1 << ADEN);
		    ADC->u8Adcsra |= (1 << ADSC);
		    while ( ADC->u8Adcsra & (1 << ADSC));
		    *Add_pu16ADCRead = (ADC->u8Adcl | (ADC->u8Adch << 8));
		    ADC->u8Adcsra &= ~(1 << ADEN);
	     }
	return RET_ErrorStatus;
}


ADC_enuErrorStatus_t ADC_enuStartConversionAsync (ADC_strChain_t *Copy_strChain)
{
	ADC_enuErrorStatus_t RET_ErrorStatus = ADC_OK;
	if(Copy_strChain == NULL)
	{
	     RET_ErrorStatus = ADC_NULL_PTR_PASSED;
	}
	else if(ADC_Working == 1)
	{
	     RET_ErrorStatus = ADC_NOT_OK; 
	}
	else
	{
		if(Copy_strChain->Result == NULL || Copy_strChain->ChannelsNumber == 0 ||Copy_strChain->ChannelsNumber == NULL)
		{
			RET_ErrorStatus = ADC_NOT_OK;
		}
		else
		{
			    strChain = Copy_strChain;	
			    ADC->u8Admux &= 0xF0;
			    ADC->u8Admux |= strChain->Channels[0];
			    ADC->u8Adcsra |= (1 << ADEN);
			    ADC->u8Adcsra |= (1 << ADSC);
			    ADC_Working = 1;
		}

	}
	return RET_ErrorStatus;
}


ADC_enuErrorStatus_t ADC_enuSetADCCallBack(void(*ADC_Callback)(void))
{
	ADC_enuErrorStatus_t RET_ErrorStatus = ADC_OK;
	if(ADC_Callback == NULL)
	{
		RET_ErrorStatus = ADC_NULL_PTR_PASSED;
	}
	else
	{
		Callback = ADC_Callback;
	}
	return RET_ErrorStatus;
}

void __vector_16(void)
{
	strChain->Result[Idx_Conversion++] = (ADC->u8Adcl | (ADC->u8Adch << 8));
	if(Idx_Conversion != strChain->ChannelsNumber)
	{
		ADC->u8Admux  &= 0xF0;
		ADC->u8Admux  |= strChain->Channels[Idx_Conversion];
		ADC->u8Adcsra |= (1 << ADSC);
	}
	else
	{
		if(Callback != NULL)
			Callback();
		ADC_Working = 0;
		Idx_Conversion = 0;
		ADC->u8Adcsra &= ~(1 << ADEN);
	}
}