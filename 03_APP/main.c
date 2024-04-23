
/*
 * Timer0App.c
 *
 * Created: 2/6/2024 10:59:57 AM
 *  Author: Mahmoud Sofar
 */ 

 #include "../01_MCAL/05_Timer1/Timer1.h"
 #include "../02_HAL/00_LED/LED.h"
 #include "../02_HAL/00_LED/LED_CFG.h"
 #include "../01_MCAL/06_ADC/ADC.h"
#include  "../01_MCAL/07_UART/Uart.h"
#include "../02_HAL/01_Switch/SWITCH.h"
 #include "../02_HAL/03_LCD/LCD.h"
#include "../01_MCAL/01_PORT/PORT.h"

#include "../01_MCAL/08_TWI/TWI.h" 
#include <avr/io.h>

#include <math.h>
#include <stdio.h> 

void * address;

uint8_t arr[2] ={'g',0}; 

 int main()
 {
	TWI_HandleTypeDef handle;
	handle.Init.GeneralCallRecogantioon = TWI_GeneralCall_Enable;
	handle.Init.interrupt		    = TWI_Interrupt_Disable;
	handle.Init.SCLFrequency	    = 500000;
	handle.Init.Prescaler		    = TWI_PRESCALLER_1;

	Uart_tstInitConfig stConfigrations;
	
	stConfigrations.u8UartIdx       = 0;
	stConfigrations.enmCharSize     = UART_SIZE_8;
	stConfigrations.enmParityType   = UART_PARITY_NONE;
	stConfigrations.u32BaudRate     = 9600;
	stConfigrations.u32SystemClock  = 8000000.0;
	stConfigrations.u8Direction     = UART_DIR_TX | UART_DIR_RX;
	stConfigrations.u8InterruptType =  UART_INTERRUPT_NONE;
	stConfigrations.enmStopBits     = UART_STOP_1;
	//handle.Init.OwnAddress		    = 0x50;
	//slave :
	handle.Init.OwnAddress		    = 0x20;
	TWI_Init(&handle);
	SWITCH_enmInit();
	LED_enmInit();

	address = Uart_pvInit(&stConfigrations);
	TWI_Mem_Write(&handle,0x50,0x000,'A',-1);
	for(uint32_t i = 0 ;i < 10000 ; i++);
	TWI_Mem_Write(&handle,0x50,0x001,'B',-1);
	for(uint32_t i = 0 ;i < 10000 ; i++);

	TWI_Mem_Write(&handle,0x50,0x002,'C',-1);
	for(uint32_t i = 0 ;i < 10000 ; i++);
	TWI_Mem_Read_IT(&handle,0x50,0x001,&arr);
	uint8_t c[2] = {'C','t'};
	while (1)
	{
		Uart_vTransmitBuffTimeout(address,&arr,1,-1,NULL);
		
	}
	
}