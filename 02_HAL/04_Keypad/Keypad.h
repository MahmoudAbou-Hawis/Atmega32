/******************************************************************************/
/**
 * @file keypad.h
 * @brief Keypad API header for ATmega32 micro-controller
 *
 * @par Project Name
 * AVR drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This header file provides function prototypes and definitions for interfacing
 * with a matrix keypad .
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/


/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef KEYPAD_H_
#define KEYPAD_H_
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
#include "../../00_LIB/Platform_Types.h"

/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/


/******************************************************************************/

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

/*
*@brief these are all LCD errors which may occur when using APIs in wrong way.
*
*/
typedef enum
{
    /**
    *@brief returned if the function did it functionality correctly.
    */
     KEYBAD_enuOK ,

     /**
     *@brief if you entered wrong port while configuration.
     */
     KEYBAD_enuInvalidPort,
	
     /**
     *@brief if you entered wrong pin while configuration.
     */
     KEYBAD_enuInvalidPin,

    /**
    *@brief returned if you pass a null pointer to functions.
    */
     KEYBAD_enuNullPtr

	
} KEYPAD_enuErrors_t;

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
 * @brief Initializes the keypad pins and registers for proper operation.
 *
 * This function performs the following actions:
 * 1. Configures the keypad port pins as inputs or outputs as required for matrix scanning.
 * 2. Enables internal pull-up resistors on the row pins, if necessary.
 * 3. Sets the initial states of the column pins.
 * 4. Configures any additional registers or settings specific to the micro-controller or keypad configuration.
 */
KEYPAD_enuErrors_t KEYPAD_enuInit(void);

/**
 * @brief Scans the keypad and returns the corresponding character if a button is pressed.
 * 
 * This function performs the following actions:
 * 1. Scans the keypad matrix to detect a button press.
 * 2. If a button is pressed, determines the corresponding character based on the keypad layout.
 * 3. Returns the character as an ASCII value in ADD_pChar.
 * 4. If no button is pressed, returns a null character (0).
 *
 * @param[in/out] ADD_pChar
 *    - Pointer to a character variable where the pressed key's ASCII value will be stored.
 *    - If a key is pressed, the function updates this variable with the corresponding character.
 *    - If no key is pressed, the variable's value is a null character (0).
 *
 * @return the Keypad error status.
 */
KEYPAD_enuErrors_t KEYPAD_enuGetKey(uint8_t * ADD_pChar);


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
#endif /* KEYPAD_H_ */
/******************************************************************************/
