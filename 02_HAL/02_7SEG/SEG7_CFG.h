/*******************************************************************************/
/**
 * @file SEG7_CFG.h
 * @brief used to configure each 7-segment in the system
 *
 * @par Project Name
 * AVR atmega32 drivers
 *
 * @par Code Language
 * C
 *
 * @par Description
 * this file to the user for configure all 7-segments in the system
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef SEG7_CFG_H_
#define SEG7_CFG_H_
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
#include "../../01_MCAL/01_PORT/PORT.h"
/******************************************************************************/

/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/

#define  NUM_OF_PINS_IN_EACH_7_SEG				7

/**
*@brief this is the number of 7 segments which used in your project
*/
#define   NUMBER_OF_7_SEGMENTS					1

/**
*@ brief this is the first 7 segment name
*/
#define	  FIRST_7_SEG						0





/*****************************************************************************/
/*				    PINS DEFINES                             */
/*****************************************************************************/



/* Define for LED A in 7-segment display */
#define PINA									0

/* Define for LED B in 7-segment display */
#define PINB 									1

/* Define for LED C in 7-segment display */
#define PINC 									2

/* Define for LED D in 7-segment display */
#define PIND 									3

/* Define for LED E in 7-segment display */
#define PINE 									4

/* Define for LED F in 7-segment display */
#define PINF 									5

/* Define for LED G in 7-segment display */
#define PING 									6

#if NUM_OF_PINS_IN_EACH_7_SEG == 8
#define PINDP									7
#endif


/******************************************************************************/

/******************************************************************************/
/* PUBLIC MACROS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC ENUMS */
/******************************************************************************/

/**
*@brief used to determine if the 7-segment is anode or cathode.
*/
typedef enum
{
	/**
	*@brief use if the 7-segment is anode.
	*/
	SEG7_enmAnode ,
	
	/**
	*@brief use if the 7-segment is cathode.
	*/
	SEG7_enmCathode
	
} SEG7_enm7SegType_t;

/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/

/**
*@brief this struct contain the configuration for each pin in 7-segment 
*/
typedef struct
{
	PORT_enmPortOPTS_t		enmPort;				/*use to determine which port you want to use Like PORT_enmPortA*/
	
	PORT_enumPins_t			enmPin;					/*use to determine which pin you want to use like PORT_enmPin1*/
	
} SEG7_st7SegPinCFG_t;

/**
*@brief this struct contain all configuration for each 7-segment 
*/
typedef struct
{
	/**
	*@brief contain all pins configuration in 7-segment.
	*/
	const SEG7_st7SegPinCFG_t acenmSetPinCFG[NUM_OF_PINS_IN_EACH_7_SEG];
	
	/**
	*@brief used to determine the 7-Segment is anode or cathode.
	*/
	SEG7_enm7SegType_t enm7SegType;
	
} SEG7_st7SegCFG_t;

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
#endif /* SEG7_CFG_H_ */
/******************************************************************************/
