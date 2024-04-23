/*******************************************************************************/
/**
* @file LCD_CFG.h
* @brief Header file for LCD configuration.
*
* @par Project Name
* AVR atmega32 drivers.
*
* @par Code Language
* C
*
* @par Description
* This header file contains configurations for the LCD (Liquid Crystal Display) module.
* Please refer to the documentation for details on how to use and customize these configurations.
*
* @par Author
* Mahmoud Abou-Hawis
*******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef LCD_CFG_H_
#define LCD_CFG_H_
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


/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/

/******************************************************************************/
/*			        LCD dimension			              */
/******************************************************************************/

#define			NUM_OF_ROWS				2

#define			NUM_OF_COLS				16




/******************************************************************************/
/*			   LCD MODE CONFIGURATION	                      */
/******************************************************************************/

/**
*@brief	 used if you want to use 4 bit mode and use only 4 wires.
*/
#define			FOUR_BIT_MODE				0

/**
*@brief used if you want to use 8 bit mode and use only 8 wires.
*/
#define			EIGHT_BIT_MODE				0x10

/**
*@brief the mode which you want to apply you must choose only 
	FOUR_BIT_MODE or EIGHT_BIT_MODE !!
*/
#define			LCD_DATE_MODE				EIGHT_BIT_MODE




/*****************************************************************************/
/*			   LCD CHARACHTER FONTS				     */
/*****************************************************************************/

/**
*@brief it is used when you want the character size 5 x 7 dots.
*/
#define			FONT_5_X_7				0

/**
*@brief it is used when you want the character size 5 x 10 dots.
*/
#define			FONT_5_X_10				0x04


/**
*@brief determine the character font size choose only FONT_5_X_7 or FONT_5_X_10.
*/
#define			LCD_CHAR_FONT				FONT_5_X_7


/******************************************************************************/
/*			  NUMBER OF LINES USED                                */
/******************************************************************************/

/**
*@brief used if you want to choose only one line to display.
*/
#define			ONE_LINE				0

/**
*@brief used if you want to choose only two line to display.
*/
#define			TWO_LINE				0x08

/**
*@brief used for determining number of lines which you want to display
*	allowed only with ONE_LINE  and TWO_LINE.
*/
#define			NUM_OF_DIS_LINES			TWO_LINE



/******************************************************************************/
/*			      CURSOR CONFIGURATION	                      */
/******************************************************************************/

/**
*@brief is used to make the property on.
*/
#define			CURSOR_ON				0x02

/**
*@brief is used to make the property off.
*/
#define			CURSOR_OFF				0


/**
*@brief to determine the cursor blinking or not.
*/
#define			CURSOR_BLINKING				1


/**
*@brief	to determine the cursor not blinking if it ON			
*/	
#define			CURSOR_NOT_BLINKING			0

/**
*@brief to determine the cursor Mode if it on or cursor not blinking. 
*/
#define			CURSOR_MODE			(CURSOR_OFF | CURSOR_NOT_BLINKING)






/******************************************************************************/
/*				 LCD PINS				      */
/******************************************************************************/


/**
*@brief refer to data pin 0 
*/
#define			D0					0

/**
*@brief refer to data pin 1
*/
#define			D1					1

/**
*@brief refer to data pin 2
*/
#define			D2					2

/**
*@brief refer to data pin 3
*/
#define			D3					3

/**
*@brief refer to data pin 4
*/
#define			D4					4

/**
*@brief refer to data pin 5
*/
#define			D5					5

/**
*@brief refer to data pin 6
*/
#define			D6					6

/**
*@brief refer to data pin 7
*/
#define			D7					7

/**
*@brief refer to Enable pin.
*/
#define			E					8

/**
*@brief refer to read/write pin.
*/
#define			RW					9

/**
*@brief refer to register select pin.
*/
#define			RS					10




/*****************************************************************************/
/*		     PLEASE DON'T CHANGE THESE CONFIGURSTIONS                */
/*****************************************************************************/

/**
*@brief this represent the number of pins in LCD.
*/
#define			NUM_PINS				11

/*******************************************************************************/

/******************************************************************************/
/* PUBLIC MACROS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC ENUMS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/

/**
*@brief used to configure each pin what is it port and pin number.
*/
typedef struct
{
      PORT_enmPortOPTS_t	enmPort;	/*use to determine which 
						port you want to use Like
						PORT_enmPortA*/

	
      PORT_enumPins_t		enmPin;		/*use to determine which pin
						you want to use like PORT_enmPin1*/
} LCD_stPinCFG_t;


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
#endif /* LCD_CFG_H_ */
/******************************************************************************/
