/******************************************************************************/
/**
 * @file Interrupt.c
 * @brief Implementation of interrupt handling functions for both external 
 * and peripheral interrupts on the AVR Atmega32 microcontroller.
 *
 * @par Project Name
 * Atmega32 Microcontroller Abstraction Layer (MCAL)
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This source file contains the implementation of functions declared in Interrupt.h
 * , providing a comprehensive set of features for managing both external and peripheral 
 * interrupts on the Atmega32 microcontroller.
 *
 * The file includes the initialization function, interrupt enabling and disabling functions,
 * and a mechanism to set user-defined callback functions for external interrupts.
 *
 *
 * This file is part of the Atmega32 Microcontroller Abstraction Layer (MCAL) project, 
 * providing a modular and clean interface for interacting with the Atmega32 microcontroller's
 * hardware features.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/


/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "Interrupt.h"
#include "Interrupt_CFG.h"
#include "../../00_LIB/BIT_MATH.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/

/******************************************************************************/
/*                    REGISTERS ADDRESSES				                      */
/******************************************************************************/

/**
*@brief Status Register is used for enable Global Interrupt.
*/
#define		SREG				*((volatile uint8_t *)0x5F)

/**
*@brief General Interrupt Control Register used for enable/disable all 
*		external interrupts.
*/
#define		GICR				*((volatile uint8_t *)0x5B)

/**
*@brief The MCU Control Register contains control bits for interrupt sense 
*		control for external interrupt 1 and external interrupt 0.
*/
#define		MCUCR				*((volatile uint8_t *)0x55)

/**
*@brief The MCU Control Register contains control bits for interrupt sense
*		control for external interrupt 2.
*/
#define		MCUCSR				*((volatile uint8_t *)0x54)

/**
*@brief General Interrupt Flag contain all external interrupts flag.
*/
#define		GIFR				*((volatile uint8_t *)0x5A)

/******************************************************************************/

/******************************************************************************/
/*	        Symbolic Constants for registers bits			      */
/******************************************************************************/

/**
*@brief Global Interrupt Enable in bit 7 int SREG register.
*/
#define		SERG_INT_EN_BIT							7			

/**
*@brief Bit 7 – INT1: External Interrupt Request 1 Enable.
*/
#define  GICR_EXT_1_INT_EN							7

/**
*@brief Bit 6 – INT0: External Interrupt Request 0 Enable.
*/
#define  GICR_EXT_0_INT_EN							6

/**
*@brief Bit 5 – INT2: External Interrupt Request 2 Enable.
*/
#define  GICR_EXT_2_INT_EN							5

/**
*@brief represent ISC00 bit in MCUCR register .
*		Interrupt Sense Control 0 bit 0.
*/
#define MCUCR_INT_SENSE_CTRL_00							0			

/**
*@brief represent ISC01 bit in MCUCR register .
*		Interrupt Sense Control 0 bit 1.
*/
#define MCUCR_INT_SENSE_CTRL_01							1

/**
*@brief represent ISC10 bit in MCUCR register .
*		Interrupt Sense Control 1 bit 0.
*/
#define MCUCR_INT_SENSE_CTRL_10							2

/**
*@brief represent ISC11 bit in MCUCR register .
*		Interrupt Sense Control 1 bit 1.
*/
#define MCUCR_INT_SENSE_CTRL_11							3		


/**
*@brief represent ISC2 bit in MCUCSR register .
*		Interrupt Sense Control 2.
*/
#define  MCUCSR_INT_SENSE_CNTL_2							6	

/**
*@brief External Interrupt Flag 1.
* when external interrupt 1 occur the bit set.
*/
#define INTF1										7

/**
*@brief External Interrupt Flag 0.
* when external interrupt 0 occur the bit set.
*/
#define INTF0										6

/**
*@brief External Interrupt Flag 2.
* when external interrupt 2 occur the bit set.
*/
#define INTF2										5

/******************************************************************************/
/*				General Defines       			      */
/******************************************************************************/
#define  NO_ACTIVITED_INT							0 

/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
/******************************************************************************/


/******************************************************************************/
/* PRIVATE ENUMS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE TYPES */
/******************************************************************************/

/**
*@brief this struct used to save call back and its params which passed with 
*		the user.
*/
typedef struct
{
	void (*ADD_CallBack)(void*);
	void * vpFuncParam;
} stIntCBCont_t;


/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

/**
*@brief contain all callbacks for all interrupts
*/
static stIntCBCont_t astCallBacks[NUM_OF_ACT_INTS];

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
#if INT0 == ENABLE
void __vector_1(void) __attribute__((signal));
#endif

#if INT1 == ENABLE
void __vector_2(void) __attribute__((signal));
#endif

#if INT2 == ENABLE
void __vector_3(void) __attribute__((signal));
#endif

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/
void EXINT_init(void)
{

	SET_BIT(SREG,SERG_INT_EN_BIT);				/*Enable global interrupt by set 
												Global Interrupt Enable in SREG */
	#if INT0 == ENABLE							/*Check if the interrupt 0 enabled*/
		SET_BIT(GICR,GICR_EXT_0_INT_EN);		/*enabled external interrupt 0 */
		uint8_t LOC_Int0Temp = 0;
		#if INT0_TYPE == FALLING_ADGE			/*if interrupt occurring in falling edge*/
		
			LOC_Int0Temp = MCUCR;		/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			SET_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_01); /*set interrupt sense control ISC01 */
			
			CLR_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_00);/*clear interrupt sense control ISC00 */
			
			MCUCR = LOC_Int0Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT0_TYPE == RAISING_ADGE
			LOC_Int0Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			SET_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_01); /*set interrupt sense control ISC01 */
			
			SET_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_00);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int0Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT0_TYPE == LOGICAL_LEVEL
			LOC_Int0Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			CLR_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_01); /*clear interrupt sense control ISC01 */
			
			SET_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_00);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int0Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT0_TYPE == LOW_LEVEL
			LOC_Int0Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			CLR_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_01); /*clear interrupt sense control ISC01 */
			
			CLR_BIT(LOC_Int0Temp , MCUCR_INT_SENSE_CTRL_00);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int0Temp;					/*Make the MCUR equal the selected interrupt type*/
		#else
			#error INT0_TYPE configure in wrong way.
		#endif		
	#endif
	
	#if INT1 == ENABLE							/*Check if the interrupt 0 enabled*/
		SET_BIT(GICR,GICR_EXT_1_INT_EN);		/*enabled external interrupt 0 */
		uint8_t LOC_Int1Temp = 0;
		#if INT1_TYPE == FALLING_ADGE			/*if interrupt occurring in falling edge*/
		
			LOC_Int1Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			SET_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_11); /*set interrupt sense control ISC01 */
			
			CLR_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_10);/*clear interrupt sense control ISC00 */
			
			MCUCR = LOC_Int1Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT1_TYPE == RAISING_ADGE
			LOC_Int1Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			SET_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_10); /*set interrupt sense control ISC01 */
			
			SET_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_11);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int1Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT1_TYPE == LOGICAL_LEVEL
			LOC_Int1Temp = MCUCR;			/*Local variable to save current state to
											 save enter not selected interrupt type*/
			
			CLR_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_11); /*clear interrupt sense control ISC01 */
			
			SET_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_10);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int1Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT1_TYPE == LOW_LEVEL
			LOC_Int1Temp = MCUCR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			CLR_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_11); /*clear interrupt sense control ISC01 */
			
			CLR_BIT(LOC_Int1Temp , MCUCR_INT_SENSE_CTRL_10);/*set interrupt sense control ISC00 */
			
			MCUCR = LOC_Int1Temp;					/*Make the MCUR equal the selected interrupt type*/
		#else
			#error INT1_TYPE configure in wrong way.
		#endif		
	#endif
	
	#if INT2 == ENABLE							/*Check if the interrupt 0 enabled*/
		SET_BIT(GICR,GICR_EXT_2_INT_EN);		/*enabled external interrupt 0 */
		uint8_t LOC_Int2Temp = 0;
		#if INT2_TYPE == FALLING_ADGE			/*if interrupt occurring in falling edge*/
		
			LOC_Int2Temp = MCUCSR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			CLR_BIT(LOC_Int2Temp , MCUCSR_INT_SENSE_CNTL_2);/*clear interrupt sense control ISC00 */
			
			MCUCSR = LOC_Int2Temp;					/*Make the MCUR equal the selected interrupt type*/
		#elif INT2_TYPE == RAISING_ADGE
			LOC_Int2Temp = MCUCSR;			/*Local variable to save current state to
												 save enter not selected interrupt type*/
			
			SET_BIT(LOC_Int2Temp , MCUCSR_INT_SENSE_CNTL_2);/*set interrupt sense control ISC00 */
			
			MCUCSR = LOC_Int2Temp;					/*Make the MCUR equal the selected interrupt type*/
		#else
			#error INT2_TYPE configure in wrong way.
		#endif		
	#endif
	
}


EXINT_enuError EXINT_enuSetCallBack(EXINT_enuInteruptsNames Copy_InterruptName, void (*ADD_CallBack)(void*),void * vpFuncParam)
{
	/*verification*/
	EXINT_enuError RET_enuErrorStatus = EXINT_enuOK;				/*return the status of function is working right or not */
	if(Copy_InterruptName >= NUM_OF_ACT_INTS					/*Check inValid interrupt name*/
	|| Copy_InterruptName < NO_ACTIVITED_INT)
	{
		RET_enuErrorStatus = EXINT_enuNotValidIntName;			/*occur when not valid name or disabled interrupt*/
	}
	else if(ADD_CallBack == NULL)
	{
		RET_enuErrorStatus = EXINT_enuNullPtr;					/*return null function pointer passed to the function*/
	}
	else
	{
		/**
		*@brief assign the  Call back and its params  for its interrupt.  
		*/
		astCallBacks[Copy_InterruptName].ADD_CallBack = ADD_CallBack;
		astCallBacks[Copy_InterruptName].vpFuncParam  = vpFuncParam;
	}
	return RET_enuErrorStatus;
}

#if INT0 == ENABLE
void __vector_1(void)
{
	if(astCallBacks[EXINT_enuEXINT0].ADD_CallBack != NULL)						/*if function not equal null execute*/
	{
		astCallBacks[EXINT_enuEXINT0].ADD_CallBack(astCallBacks[EXINT_enuEXINT0].vpFuncParam);
	}
	SET_BIT(GIFR,INT0);															/*clear interrupt flag manually*/
}
#endif


#if INT1 == ENABLE
void __vector_2(void)
{
	if(astCallBacks[EXINT_enuEXINT1].ADD_CallBack != NULL)						/*if function not equal null execute*/
	{
		astCallBacks[EXINT_enuEXINT1].ADD_CallBack(astCallBacks[EXINT_enuEXINT1].vpFuncParam);
	}
	SET_BIT(GIFR,INT1);															/*clear interrupt flag manually*/
}	
#endif


#if INT2 == ENABLE
void __vector_3(void)
{
	if(astCallBacks[EXINT_enuEXINT2].ADD_CallBack != NULL)						/*if function not equal null execute*/
	{
		astCallBacks[EXINT_enuEXINT2].ADD_CallBack(astCallBacks[EXINT_enuEXINT2].vpFuncParam);
	}
	SET_BIT(GIFR,INT2);								         	/*clear interrupt flag manually*/
}
#endif
/******************************************************************************/