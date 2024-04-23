/******************************************************************************/
/**
 * @file lcd.c
 * @brief Implementation of LCD API functions for ATmega32 micro-controller
 *
 * @par Project Name
 * AVR drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This file contains functions for initializing and controlling an LCD display
 * connected to an ATmega32 micro-controller. It supports basic text display
 * operations, including:
 *  - Initialization of the LCD module
 *  - Writing characters and strings to the display
 *  - Clearing the display
 *  - Setting the cursor position
 *  - Controlling cursor visibility
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "../../01_MCAL/00_DIO/DIO.h"
#include "../../01_MCAL/01_PORT/PORT.h"
#include "LCD.h"
#include "LCD_CFG.h"
#include "../../00_LIB/BIT_MATH.h"
#ifndef F_CPU
#define		F_CPU             8000000UL
#endif
#include <util/delay.h>
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/

/**
*@brief FUNCTION set command used to configure the number of lines , the font
*	and LCD data mode (4-bit or 8-bit).
*/
#define		 FUNCTION_SET_CMD	       (0x20 | LCD_DATE_MODE |NUM_OF_DIS_LINES | LCD_CHAR_FONT)

/**
*@brief used to set cursor in new address in DDRAM.
*/
#define		 SET_DDRAM_ADD_CMD(ADD)	       (0x80 | ADD)

/**
*@brief refer to the the DDRAM Lines .
* First Line:
*
*	Start Address: 0x00
*	End Address: 0x0F
*Second Line:
*
*	Start Address: 0x40
*	End Address: 0x4F
*/
#define		 NUMBER_OF_LINES	       2

/**
*@brief refer to the first bit in register.
*/
#define		 FIRST_BIT		       0

/**
*@brief refer to Command will sent.
*/
#define		 CMD			       0

/**
*@brief refer to data will sent.
*/
#define		 DATA			       1

/**
*@brief refer to number of bytes in CGROM.
*/
#define		NUM_CHR_BYTES_CGRAM	       8

/**
*@brief used to set cursor in new address in CGRAM.
*/
#define		SET_CGRAM_ADD_CMD(ADD)		(0x40 | ADD)
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
*@brief used to know what is the current Cursor column.
*/
static uint8_t u8CursorCol = 0;

/**
*@brief used to know what is the current Cursor column.
*/
static uint8_t u8CursorRow = 0;

/**
*@brief this array refer to the first address in each line in DDRAM.
*/
static const uint8_t au8LCDLinesBaseAdd[NUMBER_OF_LINES] =  {0x00 , 0x40};

/**
*@brief used to make internal command are allowed to sent.
*/
static boolean bAllowCMD = TRUE;

#if LCD_DATE_MODE  == FOUR_BIT_MODE
/**
*@brief used if you want to send upper bits only.
*/
    static uint8_t u8ConfFlag = 1;	
#endif 

boolean CGRAM = FALSE;
/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLE DEFINITIONS */
/******************************************************************************/

/**
*@brief refer to the configuration in LCF_CFG.h which you configured.
*/
extern const LCD_stPinCFG_t astLCD_CFG[NUM_PINS];
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES */
/******************************************************************************/

/**
*@brief used to check the boundaries of LCD.
*/
static LCD_enuErrorStatus enuCheckBoundry(void);

/**
*@brief used to write data or command in all LCD mode(8-bit or 4-bit).
*
*@param[in] Copy_bRS refer to the operation.
*	   0 --> command
*	   1 --> data
*
*@param[in] Copy_u8Data refer to the status of pins.
*
*
*@return no thing.
*/
static void vSetPins(boolean Copy_bRS,uint8_t Copy_u8Data);
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/
/******************************************************************************/

static LCD_enuErrorStatus enuCheckBoundry(void)
{

	if(CGRAM == TRUE)
	{
		return LCD_enuOK;
	}
	/*the returned value*/
	LCD_enuErrorStatus RET_enoErrorStatus = LCD_enuOK; 

	/*check if the display is full*/

	if(u8CursorCol >= NUM_OF_COLS && u8CursorRow >= NUM_OF_ROWS)
	{
		RET_enoErrorStatus = LCD_enuDisplayIsFull;
	}

	/*check if the row is full and will move the cursor to the next row.*/
	else if(u8CursorCol >= NUM_OF_COLS)
	{
		u8CursorRow++;
		u8CursorCol = 0;
		/*change the cursor to the first place in the new row.*/
		LCD_enuGotoDDRAM_XY(u8CursorRow,u8CursorCol);
	}
	else
	{
		/*NO thing*/
	}
	/*returned value of the function.*/
	return RET_enoErrorStatus;
}

static void vSetPins(boolean Copy_bRS,uint8_t Copy_u8Data)
{
	uint8_t LOC_u8Data = Copy_u8Data;
	/*LOC_u8Start refer to the the the first pin will start*/
    #if LCD_DATE_MODE == EIGHT_BIT_MODE
	uint8_t LOC_u8Start = 0;
    #elif LCD_DATE_MODE == FOUR_BIT_MODE
	uint8_t LOC_u8Start = 4;
	for(uint8_t LOC_SendTwice = 0 ; LOC_SendTwice < u8ConfFlag +1 ; LOC_SendTwice++)
	{
		LOC_u8Data = LEFT_SHIFT(Copy_u8Data , LOC_SendTwice * 4);
    #else
	#error "LCD_DATA_MODE configuration is wrong"
    #endif
    /*set data in the wires*/
    for(uint8_t LOC_u8Pin = LOC_u8Start ; LOC_u8Pin <= D7 ; LOC_u8Pin++)
    {
	DIO_enumSetPin(astLCD_CFG[LOC_u8Pin].enmPort ,
	astLCD_CFG[LOC_u8Pin].enmPin ,
	GET_BIT(LOC_u8Data,LOC_u8Pin));
    }
    /*execute the command*/
    DIO_enumSetPin(astLCD_CFG[RW].enmPort,astLCD_CFG[RW].enmPin,DIO_enumLogicLow);
    DIO_enumSetPin(astLCD_CFG[RS].enmPort,astLCD_CFG[RS].enmPin,(!Copy_bRS)?DIO_enumLogicLow:DIO_enumLogicHigh);
    DIO_enumSetPin(astLCD_CFG[E].enmPort,astLCD_CFG[E].enmPin,DIO_enumLogicHigh);
    _delay_ms(1);
    DIO_enumSetPin(astLCD_CFG[E].enmPort,astLCD_CFG[E].enmPin,DIO_enumLogicLow);
    #if LCD_DATE_MODE == FOUR_BIT_MODE
       }
    #endif 
} 
/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/


LCD_enuErrorStatus LCD_init(void)
{
    LCD_enuErrorStatus RET_enmErrorStatus = LCD_enuOK;
    PORT_enmError_t LOC_enmErrorStatus = PORT_enmOk;
    PORT_stPortCfg_t stCFG = {0};
    /*used to exit if wrong configuration detected*/		
    boolean LOC_bExit = FALSE;
    /*to configure the pins*/
    uint8_t LOC_u8St = (LCD_DATE_MODE == EIGHT_BIT_MODE)?D0:D4; 
    for (uint8_t LOC_u8Pin = LOC_u8St ; LOC_u8Pin < NUM_PINS && !LOC_bExit ; LOC_u8Pin++)
    {
	  stCFG.enmPin  = astLCD_CFG[LOC_u8Pin].enmPin;
	  stCFG.enmPort = astLCD_CFG[LOC_u8Pin].enmPort;
	  stCFG.enmPinConf = PORT_enmOutputLOW;
	 LOC_enmErrorStatus = PORT_enmSetCfg(&stCFG);
	if(LOC_enmErrorStatus == PORT_enmPortInvalid)			/*check if the port is invalid*/
	{
		RET_enmErrorStatus = LCD_enmInvalidPort;		/*return invalid status*/
		LOC_bExit = TRUE;					/*will exit the the init process*/
	}
	else if(LOC_enmErrorStatus == PORT_enmPinNumOutOfRange)		/*check if the pin out of valid range */
	{
		RET_enmErrorStatus = LCD_enmInvalidPin;			/*return the pin is not valid*/
		LOC_bExit = TRUE;					/*exit the init process*/
	}
	else
	{
		/*Do nothing*/
	}
    }
    if(LOC_bExit == FALSE)
    {
	    /*the initialization of LCD */
	    #if LCD_DATE_MODE == FOUR_BIT_MODE
	    bAllowCMD = FALSE;
	    LCD_enuWriteCommand(0x22);
	    u8ConfFlag = 0;
	    LCD_enuWriteCommand(LEFT_SHIFT((NUMBER_OF_LINES | FOUR_BIT_MODE),6));
	    u8ConfFlag = 1;
	    bAllowCMD = TRUE;
	    #endif
	    #if LCD_DATE_MODE == EIGHT_BIT_MODE
	    bAllowCMD = FALSE;
	    LCD_enuWriteCommand(FUNCTION_SET_CMD);
	    bAllowCMD = TRUE;
	    #endif
	    _delay_ms(2);
	    LCD_enuWriteCommand(DISPLAY_ON_CMD);
	    _delay_ms(2);
	    LCD_enuWriteCommand(DISPLY_CLEAR);
	    _delay_ms(2);
	    LCD_enuWriteCommand(ENTERY_MODE_SET_CMD);
	    _delay_ms(2);
	}
	else 
	{
	/* do no thing*/
	}
	/*the returned value.*/
    return RET_enmErrorStatus;
}


LCD_enuErrorStatus LCD_enuWriteData(uint8_t Copy_u8Data)
{
	/*check if the boundary have a space to write the new character.*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;

	/*if we have a place to write the new character.*/
	if(RET_enuErrorStatus == LCD_enuOK)
	{
		/*print it and increase the column by one*/
		vSetPins(DATA,Copy_u8Data);
		u8CursorCol++;
	}
	else
	{
		/* NO thing*/
	}
	/*the return of function.*/
	return RET_enuErrorStatus;
}


LCD_enuErrorStatus LCD_enuWriteCommand(uint8_t Copy_u8Command)
{
	/*refer to the function returned value*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;

	/*check if the command is not allowed.*/
	if(Copy_u8Command != DISPLY_CLEAR  && Copy_u8Command != ENTERY_MODE_SET_CMD &&Copy_u8Command !=  DISPLAY_ON_CMD &&
		Copy_u8Command != RETURN_HOME_CMD  && Copy_u8Command !=  SHIFT_CURSOR_LEFT && Copy_u8Command != SHIFT_CURSOR_RIGHT &&
		Copy_u8Command !=  SHIFT_DISPLAY_LEFT  &&Copy_u8Command !=  SHIFT_DISPLAY_RIGHT && bAllowCMD && Copy_u8Command != DISPLAY_ON_CMD)
	{
		RET_enuErrorStatus = LCD_enuNotValidCmd;
	}
	else
	{
		/**
		* Clear Display command return cursor to the home so virtual cursor must return to 0,0.
		*/
		if(Copy_u8Command == DISPLY_CLEAR)
		{
			u8CursorRow = 0;
			u8CursorCol = 0;
		}
		/*send the command*/
		vSetPins(CMD,Copy_u8Command);
	}
	
	/*the returned value*/
	return RET_enuErrorStatus;
}

LCD_enuErrorStatus LCD_enuGotoDDRAM_XY(uint8_t Copy_u8X, uint8_t Copy_u8Y)
{
	/*refer to the function returned value*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;
	/*check if the position is not valid.*/
	if(Copy_u8X >= NUM_OF_ROWS || Copy_u8Y >= NUM_OF_COLS || Copy_u8X < 0 || Copy_u8Y < 0)
	{
		RET_enuErrorStatus = LCD_enuNotValidPositionInDDRAM;
	}
	else
	{
		/*the base address of the line in DDRAM*/
		uint8_t LOC_u8BaseAdd = au8LCDLinesBaseAdd[GET_BIT(Copy_u8X,FIRST_BIT)];

		/*the offset which added to the base address to get the right address.*/
		uint8_t LOC_u8Offset  = ((Copy_u8X > 1) ? (20 + Copy_u8Y): (Copy_u8Y));

		/*the actual address in DDRAM*/
		uint8_t LOC_u8DDRAMAdd = LOC_u8BaseAdd + LOC_u8Offset;

		/*update the cursor location*/
		u8CursorRow = Copy_u8X;
		u8CursorCol = Copy_u8Y;
		/*allow this command to sent , and sent it to LCD.*/
		bAllowCMD = FALSE;
		LCD_enuWriteCommand(SET_DDRAM_ADD_CMD(LOC_u8DDRAMAdd));
		bAllowCMD = TRUE;
	}
	/*the returned value.*/
	return RET_enuErrorStatus;
}

LCD_enuErrorStatus LCD_enuWriteNumber(sint64_t Copy_u8Number)
{
	/*array to save the number digits*/
	uint8_t au8Digits[11] ;


	/*Local variable save the number*/
	sint64_t LOC_u8Temp = Copy_u8Number;

	/*The number of digits in  the number*/
	uint8_t LOC_u8Len = 0;

	if(Copy_u8Number == 0)
	{
		au8Digits[0] = 0;
		LOC_u8Len = 1;
	}

	/*check if the number is negative to put - in the screen*/
	if(Copy_u8Number < -1)
	{
	     LCD_enuWriteData('-');
	     LOC_u8Temp *=-1;	
	}
	else
	{
		/*No thing*/
	}

	/*separate the number digits*/
	while(LOC_u8Temp)
	{
		au8Digits[LOC_u8Len++] = LOC_u8Temp%10;
		LOC_u8Temp /=10;
	}
	/*refer to the function returned value*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;

	/*check if the row have a place to display the number digits*/
	if(LOC_u8Len + u8CursorCol > NUM_OF_COLS)
	{
		RET_enuErrorStatus =  LCD_enuRowNotEnough ;
	}
	else
	{
		/*display the digits in LCD.*/
		for(sint8_t LOC_u8Digit = LOC_u8Len -1 ; LOC_u8Digit >= 0 ;LOC_u8Digit--)
		{
			LCD_enuWriteData(au8Digits[LOC_u8Digit]+'0');
		}
 	}

	/*return the error status*/
	return RET_enuErrorStatus;
}

LCD_enuErrorStatus LCD_enuWriteString(char* Copy_pchPattern, uint8_t Copy_u8Length)
{
	/*refer to the function returned value*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;

	/*check if the row have a place to display the string*/
	if(Copy_u8Length + u8CursorCol > NUM_OF_COLS)
	{
	   RET_enuErrorStatus =  LCD_enuRowNotEnough ;
	}

	/*check if the parameter not equal null*/
	else if(Copy_pchPattern == NULL)
	{
	    RET_enuErrorStatus = LCD_enuNullPtr;
	}
	else
	{
		/*if the LCD have a place and the pattern not equal null display it.*/
		for(uint8_t ch = 0 ; ch < Copy_u8Length ; ch++)
		{
			RET_enuErrorStatus = LCD_enuWriteData(Copy_pchPattern[ch]);
		}
	}
	/*return the error status*/
	return RET_enuErrorStatus;
}

LCD_enuErrorStatus LCD_enuDisplaySpecialPattern(uint8_t* ADD_pu8Pattern, uint8_t Copy_u8CGRAMBlockNumber,
 uint8_t Copy_u8X, uint8_t Copy_u8Y)
{
	/*the returned value*/
	LCD_enuErrorStatus RET_enuErrorStatus = LCD_enuOK;

	/*check if the Block number is valid number*/
	if(Copy_u8CGRAMBlockNumber < 0 || Copy_u8CGRAMBlockNumber > 7)
	{
		RET_enuErrorStatus = LCD_enuWrongCGRAM_Block;
	}

	/*check if the NUll pointer passed*/
	else if(ADD_pu8Pattern == NULL)
	{
		RET_enuErrorStatus = LCD_enuNullPtr;
	}
	else
	{
		CGRAM = TRUE;
		/*detect the address which will data written in it.*/
		uint8_t LOC_CGRAMAdd = Copy_u8CGRAMBlockNumber * NUM_CHR_BYTES_CGRAM;
		/*bAllowCMD used to allow internal commands*/
		bAllowCMD = FALSE;
		/*send set CGRAM Command to LCD*/
		LCD_enuWriteCommand(SET_CGRAM_ADD_CMD(LOC_CGRAMAdd));
		bAllowCMD = TRUE;
		/*refer to each byte written in CGRAM*/
		uint8_t LOC_u8Byte=0;
		for(LOC_u8Byte = 0; LOC_u8Byte < NUM_CHR_BYTES_CGRAM;LOC_u8Byte++)
		{
			/*write the data in CGRAM and increase it*/
			LCD_enuWriteData(ADD_pu8Pattern[LOC_u8Byte]);
		}
		CGRAM = FALSE;
		/*make the cursor in the new position*/
		RET_enuErrorStatus = LCD_enuGotoDDRAM_XY(Copy_u8X,Copy_u8Y);

		/*print the special character in LCD*/
		RET_enuErrorStatus = LCD_enuWriteData(Copy_u8CGRAMBlockNumber);
		
	}
	/*returned value*/
	return RET_enuErrorStatus;
}

/******************************************************************************/