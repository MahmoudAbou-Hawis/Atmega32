/*******************************************************************************/
/**
 * @file lcd.h
 * @brief Header file for the LCD driver for ATmega32 microcontroller.
 *
 * @par Project Name
 * Atmega32 Drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This header file provides function prototypes and definitions for
 * interfacing with an LCD module (LMB162A or compatible) connected to
 * an ATmega32 microcontroller.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef LCD_H_
#define LCD_H_
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

/******************************************************************************/
/* PUBLIC MACROS */
/******************************************************************************/

/**
*@brief use this command to clear Display and cursor return to the home.
*/
#define		 DISPLY_CLEAR			0x01

/**
*@brief The cursor or blink moves to the right when increased by 1 and to the left when
*	decreased by 1.
*	it Enable the shifting too.
*/
#define		 ENTERY_MODE_SET_CMD		0x06

/**
*@brief used to turn on  the CMD.
*/
#define		 DISPLAY_ON_CMD		       (0x0C | CURSOR_MODE)

/**
*@brief used to turn off  the CMD.
*/
#define		 DISPLAY_OFF_CMD	       (0x08)

/**
*@brief Sets the DD RAM address “00H” in address counter. Return display to its original position
*	if it was shifted. DD RAM contents do not change. The cursor or the blink moves to the
*	upper left end of the display. Contents of DD RAM remain unchanged.
*/
#define		RETURN_HOME_CMD		       0x02

/**
*@brief Shifts cursor position to the left (AC is decreased by 1).
*/
#define		SHIFT_CURSOR_LEFT	       0x10

/**
*@brief Shifts cursor position to the right (AC is increased by 1).
*/
#define		SHIFT_CURSOR_RIGHT	       0x14

/**
*@brief	 Shifts the entire display to the left, cursor follows the display shift.
*/
#define		SHIFT_DISPLAY_LEFT	       0x18

/**
*@brief Shifts the entire display to the right, cursor follows the display shift.
*/
#define		SHIFT_DISPLAY_RIGHT	       0x1C			

/******************************************************************************/

/******************************************************************************/
/* PUBLIC ENUMS */
/******************************************************************************/

/*
*@brief these are all LCD errors which may occur when using APIs in wrong way.
*
*/
typedef enum
{
    /**
    *@brief returned if the function did it functionality correctly.
    */
    LCD_enuOK ,
    
    /**
    *@brief returned if you configured the pin in invalid port.
    */
    LCD_enmInvalidPort ,

    /**
    *@brief returned if you configured the pin in invalid pin.
    */
    LCD_enmInvalidPin ,

    /**
    *@brief returned if you want to move the cursor in invalid position.
    */
    LCD_enuNotValidPositionInDDRAM ,

    /**
    *@brief return if you send not allowed command .
    */
    LCD_enuNotValidCmd ,

    /**
    *@brief returned if No place to write more characters.
    */
    LCD_enuDisplayIsFull ,

    /**
    *@brief returned if you want to write a string and the row don't have place.
    */
    LCD_enuRowNotEnough ,

    /**
    *@brief returned if you pass a null pointer to functions.
    */
    LCD_enuNullPtr ,
	
    /**
    *@brief returned if you want to write in invalid CGRAM block.
    */
    LCD_enuWrongCGRAM_Block

} LCD_enuErrorStatus;
/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/


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

/**
*@brief		it is responsible for initialization of the LCD.
*
*@return	None.
*
*@note		This function must called before using any LCD APIs.
*/
LCD_enuErrorStatus LCD_init(void);

/**
*@brief		it is used for sending data to LCD.
*
*@param[in]	Copy_u8Data : data which will sent.
*
*
*@return	It Will return error status. 
*/
LCD_enuErrorStatus LCD_enuWriteData(uint8_t Copy_u8Data);

/**
*@brief		it is used for sending the commands which is defined to LCD.
*
*@param[in]	Copy_u8Command : this is the command which will sent to LCD.
*
*@return	It Will return error status. 
*/
LCD_enuErrorStatus LCD_enuWriteCommand(uint8_t Copy_u8Command);

/**
*@brief		it is used for set cursor in the specified place in LCD.
*
*
*@param[in]	Copy_u8X : describe the row which you choose in LCD.
*
*@param[in]	Copy_u8Y : describe the column which you choose in LCD.
*
*@return	It will return error status;
*/
LCD_enuErrorStatus LCD_enuGotoDDRAM_XY(uint8_t Copy_u8X, uint8_t Copy_u8Y);

/**
*@brief		it used to send an number to LCD.
*
*@param[in]	Copy_u8Number : is the number which will sent.
*
*@return	it will return error status.
*/
LCD_enuErrorStatus LCD_enuWriteNumber(sint64_t Copy_u8Number);

/**
*@brief		it is used for sending string to LCD.
*
*@param[in]	Copy_pchPattern : the string which you want to send to LCD. 
*
*@param[in]	Copy_u8Length : the length of the provided string. 
*
*@return	it will return error status.
*/
LCD_enuErrorStatus LCD_enuWriteString(char* Copy_pchPattern, uint8_t Copy_u8Length);

/**
*@brief		used for to create the new special pattern and display it
*		in the determined coordinates.
*
*@param[in]	Copy_pu8Pattern : tis is the special pattern which you want
*		to generate.
*
*@param[in]	Copy_u8CGRAMBlockNumber	the address which special pattern saved.
*
*@param[in]	Copy_u8X : describe the row which you choose in LCD.
*
*@param[in]	Copy_u8Y : describe the column which you choose in LCD.
*
*@return	It will return error status;
*/
LCD_enuErrorStatus LCD_enuDisplaySpecialPattern(uint8_t* Copy_pu8Pattern,
						uint8_t Copy_u8CGRAMBlockNumber,
						uint8_t Copy_u8X,
						uint8_t Copy_u8Y);
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
#endif /* LCD_H_ */
/******************************************************************************/
