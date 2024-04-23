/******************************************************************************/
/**
 * @file keypad.c
 * @brief Implementation of keypad API functions for ATmega32 micro-controller
 *
 * @par Project Name
 * AVR drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This file contains functions for interfacing with a matrix keypad on an ATmega32
 * micro-controller. It provides operations for:
 *  - Initializing the keypad pins and configuration
 *  - Scanning the keypad matrix to detect a button press
 *  - Reading the pressed key and returning its corresponding character
 *  - Handling invalid key presses or no press at all
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "Keypad.h"
#include "Keypad_CFG.h"
#include "../../01_MCAL/00_DIO/DIO.h"
#include "../../01_MCAL/01_PORT/PORT.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/


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

/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLE DEFINITIONS */
/******************************************************************************/

/**
*@brief contain all ports and information about columns and rows to configure.
*/
extern const KEYPAD_stKeypadCFG_t KEYPAD_CFG_stCFG[KEYPAD_COLS + KEYPAD_ROWS];

/**
*@brief contain all keypad characters which is configured before. 
*/
extern const char Keypad_buttons[KEYPAD_ROWS][KEYPAD_COLS];

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

KEYPAD_enuErrors_t KEYPAD_enuInit(void)
{
	/*it is the return of the function it contain the error status of the function.*/
	KEYPAD_enuErrors_t RET_enuErrorStatus = KEYBAD_enuOK;
	
	/*used to configure the pin port and its number. */
	PORT_stPortCfg_t LOC_stPinCFG		= {0};				

	/*used in for loop and pointer to each row to configure all rows in keypad.*/
	uint8_t LOC_u8Row = 0;

	/* loop for each row in keypad to configure it as poll-up resistor */
	for(LOC_u8Row = 0 ; LOC_u8Row < KEYPAD_ROWS ; LOC_u8Row++)
	{	
		/*refer to the pin row number which is configured before*/
		LOC_stPinCFG.enmPin = KEYPAD_CFG_stCFG[LOC_u8Row].enmPin;

		/*refer to the port of the row which is configured before*/
		LOC_stPinCFG.enmPort = KEYPAD_CFG_stCFG[LOC_u8Row].enmPort;

		/*configure the row as input pull-up*/
		LOC_stPinCFG.enmPinConf = PORT_enumInputInternalPullUp;

		/*configure the row and return the error status from the port.*/
		RET_enuErrorStatus = PORT_enmSetCfg(&LOC_stPinCFG);
	}
	
	/*used in for loop and pointer to each column to configure all rows in keypad.*/
	uint8_t LOC_u8Col = KEYPAD_ROWS;

	/* loop for each row in keypad to configure it as Out put high*/
	for(LOC_u8Col ; LOC_u8Col < (KEYPAD_ROWS + KEYPAD_COLS) ; LOC_u8Col++)
	{
		/*refer to the pin column number which is configured before*/
		LOC_stPinCFG.enmPin = KEYPAD_CFG_stCFG[LOC_u8Col].enmPin;

		/*refer to the port of the column which is configured before*/
		LOC_stPinCFG.enmPort = KEYPAD_CFG_stCFG[LOC_u8Col].enmPort;

		/*configure the column as output high*/
		LOC_stPinCFG.enmPinConf = PORT_enmOutputHigh;

		/*configure the row and return the error status from the port.*/
		RET_enuErrorStatus = PORT_enmSetCfg(&LOC_stPinCFG);
	}

	/*to return the error status*/
	return RET_enuErrorStatus;
}

KEYPAD_enuErrors_t KEYPAD_enuGetKey(uint8_t * ADD_pChar)
{
	/*it is the return of the function it contain the error status of the function.*/
	KEYPAD_enuErrors_t RET_enuErrorStatus = KEYBAD_enuOK;

	/*if ADD_pChar is null the function will return, because it is an error.*/
	if(ADD_pChar == NULL)
	{
		/*return null pointer */
		RET_enuErrorStatus = KEYBAD_enuNullPtr;
	}
	else
	{
		/*this variable contains the status of the row if it high or low*/
		uint8_t LOC_u8RowStatus = 1;

		/*used in for loop and pointer to each column in keypad*/
		uint8_t LOC_u8Col = KEYPAD_ROWS;

		/*used to stop the scanning for pressed button because the button found*/
		boolean LOC_bIsBuPresed = FALSE;

		/* loop for each row in keypad to make the column as OUTPUT low*/
		for(LOC_u8Col ; LOC_u8Col < (KEYPAD_ROWS + KEYPAD_COLS) &&!LOC_bIsBuPresed ; LOC_u8Col++)
		{
			/*make the current column is output low*/
			DIO_enumSetPin(KEYPAD_CFG_stCFG[LOC_u8Col].enmPort,
				       KEYPAD_CFG_stCFG[LOC_u8Col].enmPin,
					DIO_enumLogicLow);
			
			/*used in for loop and pointer to each row in keypad.*/
			uint8_t LOC_u8Row = 0;

			/** loop for each row in keypad to read the status of the row
			 *  if the pin is pressed the the row will be low.
			 *  else the pin is will be high.
			 */
			for(LOC_u8Row = 0 ; LOC_u8Row < KEYPAD_ROWS && !LOC_bIsBuPresed ; LOC_u8Row++)
			{
				DIO_enumGetState(KEYPAD_CFG_stCFG[LOC_u8Row].enmPort,
						KEYPAD_CFG_stCFG[LOC_u8Row].enmPin,
						 &LOC_u8RowStatus);
				if(LOC_u8RowStatus == 0)
				{
					*ADD_pChar = Keypad_buttons[LOC_u8Row][LOC_u8Col % KEYPAD_COLS];
					LOC_bIsBuPresed = TRUE;
				}
				else
				{
					*ADD_pChar = 0;
				}
			}

			/*return the column to default state*/
			DIO_enumSetPin(KEYPAD_CFG_stCFG[LOC_u8Col].enmPort,
				       KEYPAD_CFG_stCFG[LOC_u8Col].enmPin,
		           	       DIO_enumLogicHigh);
		}
	}
	/*to return the error status*/
	return RET_enuErrorStatus;
}

/******************************************************************************/