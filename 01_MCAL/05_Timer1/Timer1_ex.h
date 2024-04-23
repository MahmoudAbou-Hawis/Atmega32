/*
 * Timer1_ex.h
 *
 * Created: 2/29/2024 9:08:07 PM
 *  Author: Mahmoud Sofar
 */ 


#define		TCCR1A			*((volatile uint8_t*)0x4F)

#define		TCCR1B			*((volatile uint8_t*)0x4E)

#define		TCNT1			*((volatile uint16_t*)0x4C)

#define		OCR1A			*((volatile uint16_t*)0x4A)

#define		TIMSK			*((volatile uint8_t*)0x59)


#define		SREG			*((volatile uint8_t *)0x5F)

 #define        OCIE1A			4

 #define        TOIE1			2

 #define	SERG_INT_EN_BIT		7


 /* TCCR1A */
 #define COM1A1  7
 #define COM1A0  6
 #define COM1B1  5
 #define COM1B0  4
 #define FOC1A   3
 #define FOC1B   2
 #define WGM11   1
 #define WGM10   0

 /* TCCR1B */
 #define ICNC1   7
 #define ICES1   6
 /* bit 5 reserved */
 #define WGM13   4
 #define WGM12   3
 #define CS12    2
 #define CS11    1
 #define CS10    0


