/*
 * Timer_CFG.h
 *
 * Created: 2/5/2024 6:56:01 PM
 *  Author: Mahmoud Sofar
 */ 


#ifndef TIMER0_CFG_H_
#define TIMER0_CFG_H_



/************************************************************************/
/*                       Clock configuration                            */
/************************************************************************/
#ifndef F_CPU
#define			F_CPU             8000000UL
#endif

/************************************************************************/
/*			 COMPILATION MODE                               */
/************************************************************************/

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



/**
*@brief That configuration must be fill if you use the PRE_COMPILATION_MODE
*/

/************************************************************************/
/*                        TIMER0 configuration                         */
/************************************************************************/

/**
*@brief this is all timer modes
*/
#define		NOT_PWM_TIMER	0
#define		PWM		1


#define		TIMER_MODE	NOT_PWM_TIMER


#define		NO_PRESCALLER	1

#define		PRESCALLER_8	8

#define		PRESCALLER_64	64

#define		PRESCALLER_256	256

#define		PRESCALLER_1024	1024


#define		PRESCALLER	PRESCALLER_1024



#endif /* TIMER_CFG_H_ */