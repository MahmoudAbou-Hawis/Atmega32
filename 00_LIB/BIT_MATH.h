/*
 * BIT_MATH.h
 *
 * Created: 11/27/2023 4:37:44 PM
 *  Author: Mahmoud Sofar
 */ 


#ifndef BIT_MATH_H_
#define BIT_MATH_H_



#define 	SET_BIT(REG,BITNO)		(REG |= ( 1 << BITNO ))
#define 	CLR_BIT(REG,BITNO)		(REG &= ~(1 << BITNO))
#define 	TOG_BIT(REG,BITNO)		(REG ^= (1 << BITNO))
#define 	GET_BIT(REG,BITNO)		(1& (REG >> BITNO))

#define 	SET_LOW_NIBBL(REG)		(REG |= 0x0F)
#define 	CLR_LOW_NIBBL(REG)		(REG &= ~0x0F)
#define 	TOG_LOW_NIBBL(REG)		(REG ^= 0x0F)
#define 	GET_LOW_NIBBL(REG)		(REG & 0x0F)
#define 	ASS_LOW_NIBBLE(REG,VAL) 	(REG = (REG & ~(0xF)) | (VAL & 0xF);)

#define 	SET_HIGH_NIBBL(REG)		(REG |= 0xF0)
#define 	CLR_HIGH_NIBBL(REG)		(REG &= 0x0F)
#define 	TOG_HIGH_NIBBL(REG)		(REG ^= 0xF0)
#define 	GET_HIGH_NIBBL(REG)		(REG & 0xF0)
#define 	ASS_HIGH_NIBBLE(REG,VAL) 	(REG=(REG & ~0xF) | ((VAL >> 4) & 0xF))

#define		RIGHT_SHIFT(REG,NO)		(REG >> NO)
#define		LEFT_SHIFT(REG,NO)		(REG << NO)
#define		CIRCULAR_LSHIFT(REG,NO)		((REG) << (NO) | (REG) >> (sizeof(REG) * 8 - (NO)))
#define		CIRCULAR_RSHIFT(REG,NO)		((REG) >> (NO) | (REG) << (sizeof(REG) * 8 - (NO)))

#define     SET_REG(REG)			(REG = 0XFF)
#define     CLR_REG(REG)			(REG = 0)
#define     GET_REG(REG)			(REG)
#define	    SET_BIT_FIELD(REG ,VAL, SHIFT)	REG |= (VAL << SHIFT)

#endif /* BIT_MATH_H_ */