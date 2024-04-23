/*******************************************************************************/
/**
 * @file PORT_CONFIG.h
 * @brief This contain all Configuration of ports in atmega series
 *
 * @par Project Name
 *	Port configuration
 *
 * @par Code Language
 * C
 *
 * @par Description
 *
 *	if you want to configure all ports in pre-compilation or post compilation 
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 ******************************************************************************/


/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef PORT_CONFIG_H_
#define PORT_CONFIG_H_
/******************************************************************************/


/******************************************************************************/
/* PUBLIC DEFINES */
/******************************************************************************/


/*
*@brief Number of Ports in Atmega series
*
* @par Description it should be 3 or 4
*/
#define  NUMBER_OF_PORTS		 4

#define NUMBER_OF_PINS_IN_EACH_PORT     8

/******************************************************************************/



/******************************************************************************/
/*
*@brief configurations  Mode options 
*/
/******************************************************************************/

/*
* @brief to activate pre-compilation mode
*/
#define PRE_COMPILATION_MODE   0

/*
* @brief to activate post-compilation mode
*/
#define POST_COMPILATION_MODE  1
/******************************************************************************/


/*
* @brief To determine your  configuration is pre-compilation or post-compilation
*
*@par description 
* Your options is :
*1- PRE_COMPILATION_MODE
*2- POST_COMPILATION_MODE
*/
#define COMPILATION_MODE    POST_COMPILATION_MODE


/******************************************************************************/
/*
* @brief configuration pis options 
*
*/
/******************************************************************************/

/**
* @brief   make the the pin is input and activate pull-down
*/
#define DEAFULT						0

/**
* @brief  This will configure the pin as output and it will be high (5V)
*/
#define OUTPUT_HIGH					1

/*
* @brief This will configure the pin as input and active pull-up
*/
#define INPUT_INERNAL_PULL_UP		2

/*
* @brief This will configure the pin as input and active pull-Down
*/
#define INPUT_INERNAL_PULL_DOWN		3

/*
* @brief This will configure the pin as output and it will be low  (0V)
*/
#define OUTPUT_LOW					4


/*******************************************************************************************/


/******************************************************************************/

#endif /* PORT_CONFIG_H_ */
/******************************************************************************/