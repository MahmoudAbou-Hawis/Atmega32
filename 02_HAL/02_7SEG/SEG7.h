/*******************************************************************************/
/**
 * @file SEG7.h
 * @brief contain all function to control 7 segment
 *
 * @par Project Name
 * 7 segment 
 *
 * @par Code Language
 * C
 *
 * @par Description
 * you can use this driver for control 7 segment to show number in specific 
 * 7 segment ,or show the number in all connected 7 segments. you must 
 * put all 7 segments form left to right, and left one is the first one
 * in the configuration.
 *	
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef SEG7_H_
#define SEG7_H_
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
#include "../../00_LIB/BIT_MATH.h"
#include "../../01_MCAL/00_DIO/DIO.h"
#include "SEG7_CFG.h"

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

/**
*@brief	enum contain all errors which can returned by 7 Segment driver
*/
typedef enum
{
	/**
	*@brief  if the function done correctly
	*/
	SEG7_enmOK ,
	
	/**
	*@brief if you entered wrong port while configuration 
	*/
	SEG7_enmInvalidPort,
	
	/**
	*@brief if you entered wrong pin while configuration 
	*/
	SEG7_enmInvalidPin,
	
	
	/**
	*@brief if you entered wrong 7-segment which not configured .
	*/
	SEG7_enmInvalid7SEG ,
	
	/**
	*@brief if you configured a pin is input in another module .
	*/
	SEG7_enmConfiguredInputInAnotherModule,
	
	/**
	*@brief if the number can't displayed in the connected 7-segments
	*/
	SEG7_enmHugeNumber,
	
	/**
	*@brief if the number can't displayed in the connected 7-segments
	*/
	SEG7_enmSmallNumber,
	
	/**
	*@brief if the 7-segment not configured as Anode or cathode 
	*/
	SEG7_enmWrong7SegType
	
} SEG7_enuErrors_t;

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

/*----------------------------------------------------------------------------*/
/**
 * @brief							used to initialize all 7 segments which you 
*									configured in SEG7_CFG.h
 * 	
 * @return							enum contain Error State
 * 
 * @note							you must configure all 7 segments in SE7_CFG.h.
/*----------------------------------------------------------------------------*/
SEG7_enuErrors_t SEG7_enmInit(void);


/*----------------------------------------------------------------------------*/
/**
 * @brief							used to set value in specific segment with  
 *									its name.
 *
 * @param[in]  	Copy_cu8SegName		you should put the 7 segment name which
 *									you configured in SEG7_CFG.h as an argument
 *									when you calling this function.
 *
 * @param[in]	Copy_cu8Value		this is the number value which will display
 *									in the selected 7 segment.
 *
 *
 * @return							enum contain Error State.
 * 
 * @note							you must configure 7 segments in SE7_CFG.h.
/*----------------------------------------------------------------------------*/
SEG7_enuErrors_t SEG7_enuSetValue(const uint8_t Copy_cu8SegName
									, const uint8_t Copy_cu8Value);
									



/*----------------------------------------------------------------------------*/
/**
 * @brief							used to set value in all 7 segments which
 *									configured.
 * 	
 * @param[in]	Copy_cu8Value		this is the number value which will display
 *									in the all 7 segments.	
 * 
 *
 * @return							enum contain Error State.
 * 
 * @note							the number which will display must number of
									its digits equal to the number of 7 segments.
/*----------------------------------------------------------------------------*/
SEG7_enuErrors_t SEG7_enuSetValueAll(sint32_t Copy_cu8Value);

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
#endif /* SEG7_H_ */
/******************************************************************************/
