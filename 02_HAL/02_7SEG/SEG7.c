/******************************************************************************/
/**
 * @file FILE.c
 * @brief 
 *
 * @par Project Name
 * 
 *
 * @par Code Language
 * C
 *
 * @par Description
 * 
 * 
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "SEG7.h"
#include "../../00_LIB/math.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/
/**
 *@brief the minimum number of attached 7-segments is 1
 */
#define ONE_7_SEG_ATTACHED						0

/**
 *@brief the maximum elements can be displayed in one 7-segment
 */
#define NUM_ElEMENTS_IN_7_SEG					10

/**
 *@brief the minimum number can be displayed in one 7-segment
 */
#define MIN_NUM_IN_7_SEG						0


/**
 *@brief number of numbers which can be displayed in 7-segment (0-9) and  '-'
 *
 */
#define NUM_OF_NUMS_CAN_BE_DIS					11
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


/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

/**
*@brief configured with user in SEG7_CFG.h 
*/
extern const SEG7_st7SegCFG_t SEG7_cast7SegsCFG[NUM_OF_NUMS_CAN_BE_DIS];

/**
*@brief this contain the max number can display in 7-segments , it is determined 
*		when application running. 
*/
static sint32_t u16MaxDisplayableNum  = 0;

/**
*@brief this contain the min number can display in 7-segments , it is determined
*		when application running.
*/
static sint32_t s16MinDisplayableNum  = 0;


/**
*@brief used to decode the number which passed as argument so we can display it.
*
*/
static const uint8_t DecodeEachDigitTo7Seg[NUM_OF_NUMS_CAN_BE_DIS] = 
{
	/*
	*@brief represent zero in 7-segment
	*/
	63 ,
	
	/*
	*@brief represent 1 in 7-segment
	*/
	6 ,
	
	/*
	*@brief represent 2 in 7-segment
	*/
	91 ,
	
	/*
	*@brief represent 3 in 7-segment
	*/
	79 ,
	
	/*
	*@brief represent 4 in 7-segment
	*/
	102,
	
	/*
	*@brief represent 5 in 7-segment
	*/
	109 ,
		
	/*
	*@brief represent 6 in 7-segment
	*/
	125 ,
	
	/*
	*@brief represent 7 in 7-segment
	*/
	7 ,
	
	/*
	*@brief represent 8 in 7-segment
	*/
	127 ,
	
	/*
	*@brief represent 9 in 7-segment
	*/
	111 ,
	
	/*
	*@brief represent - in 7-segment
	*/
	64
	
};
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

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/

SEG7_enuErrors_t SEG7_enmInit(void)
{
	SEG7_enuErrors_t RET_enmErrorStatus = SEG7_enmOK;						/*return of the function which initialized with No problem */				
	PORT_enmError_t LOC_enmErrorStatus = PORT_enmOk;						/*return of function PORT_enmSetCfg which initialized with No problem*/
	boolean LOC_bExit					= FALSE;							/*to exit the function in found any configuration problem*/
	PORT_stPortCfg_t LOC_stPinCFG = {0};									/*used by port to configure each pin in 7-segment*/
	for(uint8_t LOC_u87Seg = 0 ; LOC_u87Seg < NUMBER_OF_7_SEGMENTS && !LOC_bExit ; ++LOC_u87Seg)	/*loop in each 7-segment*/
	{
		/*Validation*/
		if(SEG7_cast7SegsCFG[LOC_u87Seg].enm7SegType < SEG7_enmAnode ||								/*Check if 7-segment type is not valid*/
		SEG7_cast7SegsCFG[LOC_u87Seg].enm7SegType > SEG7_enmCathode )
		{
			RET_enmErrorStatus  = SEG7_enmWrong7SegType;												/*return the 7-segment type wrong*/
			LOC_bExit			= TRUE;																/*exit loop*/
		}
		else
		{
			LOC_stPinCFG.enmPinConf= ((SEG7_cast7SegsCFG[LOC_u87Seg].enm7SegType == SEG7_enmAnode)?		/*to make all LEDs in 7-segments low*/
			PORT_enmOutputHigh:PORT_enmOutputLOW);
			for(uint8_t LOC_u8Pin = 0 ; LOC_u8Pin < NUM_OF_PINS_IN_EACH_7_SEG && !LOC_bExit; ++LOC_u8Pin )	/*loop in each pin for the each 7-segment*/
			{
				LOC_stPinCFG.enmPin	= SEG7_cast7SegsCFG[LOC_u87Seg].acenmSetPinCFG[LOC_u8Pin].enmPin;		/*init the configuration led for the port */
				LOC_stPinCFG.enmPort	= SEG7_cast7SegsCFG[LOC_u87Seg].acenmSetPinCFG[LOC_u8Pin].enmPort;
				LOC_enmErrorStatus = PORT_enmSetCfg(&LOC_stPinCFG);						/*Trying to set the pin configuration*/
				if(LOC_enmErrorStatus == PORT_enmPortInvalid)							/*check if the port is invalid*/
				{
					RET_enmErrorStatus = SEG7_enmInvalidPort;							/*return invalid status*/
					LOC_bExit = TRUE;													/*will exit the the init process*/
				}
				else if(LOC_enmErrorStatus == PORT_enmPinNumOutOfRange)					/*check if the pin out of valid range */
				{
					RET_enmErrorStatus = SEG7_enmInvalidPin;							/*return the pin is not valid*/
					LOC_bExit = TRUE;													/*exit the init process*/
				}
				else
				{
					/*Do nothing*/
				}
			}

			
		}
	}
	u16MaxDisplayableNum = power(10,NUMBER_OF_7_SEGMENTS) - 1;								/*to calculate the max number can be displayed*/
	s16MinDisplayableNum = -(u16MaxDisplayableNum /10);											/*to calculate the minimum number can be displayed*/
	return RET_enmErrorStatus;															/*the return of the function*/
}



SEG7_enuErrors_t SEG7_enuSetValue(const uint8_t Copy_cu8SegName 
									, const uint8_t Copy_cu8Value)
{
	SEG7_enuErrors_t RET_enmErrorStatus = SEG7_enmOK;						/*return of the function which initialized with No problem */	
	DIO_enumError_t LOC_enmErrorsStatus = DIO_enmOk;						/*return of function DIO_enumSetPin error.initialized with No problem  */
	DIO_enmPortOPTS LOC_enuPort = SEG7_cast7SegsCFG[Copy_cu8SegName].acenmSetPinCFG[PINA].enmPort; /*save the port configuration for each pin*/
	SEG7_enm7SegType_t LOC_7SegType = SEG7_cast7SegsCFG[Copy_cu8SegName].enm7SegType;			   /*variable for 7-segment type*/
	boolean LOC_bExit = FALSE;																	   /*to exit if is found any error in any pin*/
	/*Validation*/
	if(Copy_cu8SegName >= NUM_OF_PINS_IN_EACH_7_SEG  || Copy_cu8SegName < ONE_7_SEG_ATTACHED)	/*Check if the Copy_cu8SegName is Not valid*/
	{
		RET_enmErrorStatus = SEG7_enmInvalid7SEG;													/*7-segment is not found*/
	}
	else if(Copy_cu8Value > NUM_ElEMENTS_IN_7_SEG)													  /*Number is more then 10*/
	{
		RET_enmErrorStatus = SEG7_enmHugeNumber;													/*return the number is Huge */
	}
	else if(Copy_cu8Value < MIN_NUM_IN_7_SEG )												    	/*Number is less then 0*/
	{
		RET_enmErrorStatus = SEG7_enmSmallNumber;													/*return the number is small */
	}
	else
	{
		for(uint8_t LOC_u8Pin = 0 ; LOC_u8Pin < NUM_OF_PINS_IN_EACH_7_SEG && !LOC_bExit ; LOC_u8Pin++)			/*loop for each pin in 7-segment */
		{
			DIO_enmLOutputPinState_t LOC_enuPinState = ((														/*to find the pin state*/
			(LOC_7SegType == SEG7_enmAnode && !GET_BIT(DecodeEachDigitTo7Seg[Copy_cu8Value] , LOC_u8Pin)) ||
			(LOC_7SegType == SEG7_enmCathode && GET_BIT(DecodeEachDigitTo7Seg[Copy_cu8Value] , LOC_u8Pin) ) 
			 )?DIO_enumLogicHigh : DIO_enumLogicLow);
			 LOC_enuPort = SEG7_cast7SegsCFG[Copy_cu8SegName].acenmSetPinCFG[LOC_u8Pin].enmPort;				/*the pin port*/
			 LOC_enmErrorsStatus = DIO_enumSetPin(LOC_enuPort,LOC_u8Pin,LOC_enuPinState);						/*set the pin state*/
			if(LOC_enmErrorsStatus == DIO_enmPinIsInputNotOutput)						/*if the DIO_enumSetPin Failed to set the pin*/
			{
				RET_enmErrorStatus = SEG7_enmConfiguredInputInAnotherModule;			/*The pin is INPUT not OUTPUT*/
				LOC_bExit = TRUE;														/*exit the loop and return the error*/
			}
			else
			{
				/* No Thing*/
			}
		}
	}
	
	return RET_enmErrorStatus;																	/*return of the function*/
}


/*
*@brief Copy_cu8Value must be sint32_t to cover all numbers in 
*		the range for example we have 3 7-segments in our system
*		the range is -99 -> 999. if it sint8_t -1 == 255
*		so we can't detect if the number is negative or positive  
*/
SEG7_enuErrors_t SEG7_enuSetValueAll(sint32_t Copy_cu8Value)
{
	SEG7_enuErrors_t RET_enuErrorStatus    = SEG7_enmOK;							/*return of the function which initialized with No problem */
	if(Copy_cu8Value > u16MaxDisplayableNum)											/* check if the number can't be displayed */
	{
		RET_enuErrorStatus = SEG7_enmHugeNumber;									/*return error the number is huge*/
	}
	else if(Copy_cu8Value < s16MinDisplayableNum)										/* check if the number can't be displayed */
	{
		RET_enuErrorStatus = SEG7_enmSmallNumber;									/*return error the number is small */ 
	}
	else
	{
		boolean LOC_bNeg = (Copy_cu8Value < 0) ? TRUE:FALSE;						/*Check if the number is negative*/
		if(LOC_bNeg == TRUE)
		{
			Copy_cu8Value = -Copy_cu8Value;
		}
		uint8_t u8Number = 0;
		for(uint8_t LOC_u8Seg = 0 ; LOC_u8Seg < NUMBER_OF_7_SEGMENTS -1 ;LOC_u8Seg++)
		{
			u8Number = Copy_cu8Value %10;
			if(Copy_cu8Value != 0)
			{	 
				Copy_cu8Value /=10;
			}
			SEG7_enuSetValue(LOC_u8Seg,u8Number);
		}
		if(LOC_bNeg == TRUE)
		{
			SEG7_enuSetValue(NUMBER_OF_7_SEGMENTS -1,NUM_ElEMENTS_IN_7_SEG);
		}
		else
		{
			SEG7_enuSetValue(NUMBER_OF_7_SEGMENTS-1,Copy_cu8Value);
		}
	}
	return RET_enuErrorStatus;
}

/******************************************************************************/

