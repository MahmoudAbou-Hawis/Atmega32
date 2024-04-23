/******************************************************************************/
/**
 * @file <file-name>.c
 * @brief <file-short-description>
 *
 * @par Project Name
 * <project-name>
 *
 * @par Code Language
 * C
 *
 * @par Description
 * <file-long-description>
 * 
 * @par Author
 * Mahmoud Abou-Hawis
 *
 * @data 16-4-2024
 */
/******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/
#include "TWI.h"
#include <math.h>
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/
#define TWI_BASE_ADD				((uint8_t)0x20)

#define TWI					((TWI_TypeDef*)TWI_BASE_ADD)

#define CPU_FREQ				((uint32_t)8000000)

/* TWCR */
#define TWINT   7
#define TWEA    6
#define TWSTA   5
#define TWSTO   4
#define TWWC    3
#define TWEN    2
/* bit 1 reserved */
#define TWIE    0

/* TWSR */
#define TWS7    7
#define TWS6    6
#define TWS5    5
#define TWS4    4
#define TWS3    3
/* bit 2 reserved */
#define TWPS1   1
#define TWPS0   0

#define		SREG			*((volatile uint8_t *)0x5F)


/**
 * @brief Start condition transmitted.
 */
#define TWI_START               0x08    

/**
 * @brief Repeated start condition transmitted.
 */
#define TWI_RESTART             0x10    

/**
 * @brief Master transmitter mode, SLA+W transmitted, ACK received.
 */
#define TWI_MT_SLA_ACK          0x18    

/**
 * @brief Master transmitter mode, SLA+W transmitted, NACK received.
 */
#define TWI_MT_SLA_NACK         0x20    

/**
 * @brief Master transmitter mode, data transmitted, ACK received.
 */
#define TWI_MT_DATA_ACK         0x28    

/**
 * @brief Master transmitter mode, data transmitted, NACK received.
 */
#define TWI_MT_DATA_NACK        0x30    

/**
 * @brief Master arbitration lost in SLA+W or data byte.
 */
#define TWI_MT_ARB_LOST         0x38    

/**
 * @brief Master receiver mode, SLA+R transmitted, ACK received.
 */
#define TWI_MR_SLA_ACK          0x40    

/**
 * @brief Master receiver mode, SLA+R transmitted, NACK received.
 */
#define TWI_MR_SLA_NACK         0x48    

/**
 * @brief Master receiver mode, data received, ACK transmitted.
 */
#define TWI_MR_DATA_ACK         0x50    

/**
 * @brief Master receiver mode, data received, NACK transmitted.
 */
#define TWI_MR_DATA_NACK        0x58    

/**
 * @brief Slave receiver mode, SLA+W received, ACK transmitted.
 */
#define TWI_SR_SLA_ACK          0x60    

/**
 * @brief Arbitration lost in SLA+R/W as master, SLA+W received, ACK transmitted.
 */
#define TWI_SR_ARB_LOST_SLA_ACK    0x68    

/**
 * @brief Slave receiver mode, general call address received, ACK transmitted.
 */
#define TWI_SR_GCALL_ACK        0x70    

/**
 * @brief Arbitration lost in SLA+R/W as master, general call address received, ACK transmitted.
 */
#define TWI_SR_ARB_LOST_GCALL_ACK  0x78    

/**
 * @brief Slave receiver mode, data received, ACK transmitted.
 */
#define TWI_SR_DATA_ACK         0x80    

/**
 * @brief Slave receiver mode, data received, NACK transmitted.
 */
#define TWI_SR_DATA_NACK        0x88    

/**
 * @brief Slave receiver mode, general call data received, ACK transmitted.
 */
#define TWI_SR_GCALL_DATA_ACK   0x90    

/**
 * @brief Slave receiver mode, general call data received, NACK transmitted.
 */
#define TWI_SR_GCALL_DATA_NACK  0x98    

/**
 * @brief Slave transmitter mode, SLA+R received, ACK transmitted.
 */
#define TWI_ST_SLA_ACK          0xA8    

/**
 * @brief Arbitration lost in SLA+R/W as master, SLA+R received, ACK transmitted.
 */
#define TWI_ST_ARB_LOST_SLA_ACK    0xB0    

/**
 * @brief Slave transmitter mode, data transmitted, ACK received.
 */
#define TWI_ST_DATA_ACK         0xB8    

/**
 * @brief Slave transmitter mode, data transmitted, NACK received.
 */
#define TWI_ST_DATA_NACK        0xC0    

/**
 * @brief Slave transmitter mode, last data byte transmitted (AA = 0); ACK received.
 */
#define TWI_ST_LAST_DATA        0xC8    


/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
/******************************************************************************/
#define CALCULATE_TWBR(SCL_FREQ, TWPS) (((((CPU_FREQ / SCL_FREQ) - 16UL))) / (2 * (1 << (2 * TWPS))))


/******************************************************************************/

/******************************************************************************/
/* PRIVATE ENUMS */
/******************************************************************************/
typedef enum
{
	WRITE_EEPROM_DATA,
	READ_EEPROM_DATA
} Service;

/******************************************************************************/

/******************************************************************************/
/* PRIVATE TYPES */
/******************************************************************************/

typedef struct
{
	volatile uint8_t TWBR;
	volatile uint8_t TWSR;
	volatile uint8_t TWAR;
	volatile uint8_t TWDR;
	uint8_t padding[50];  
	volatile uint8_t TWCR;
} TWI_TypeDef;

typedef struct  
{
	uint8_t DevAddress;
	uint8_t MemAddress;
	uint8_t data;
	uint8_t cnt ;
} TWI_info;

/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/
static TWI_HandleTypeDef * TWI_Handle = NULL;
static TWI_info EEPROM_Write;
/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/
static uint8_t TWI_Busy = 0;
static Service service = 0;
/******************************************************************************/

/******************************************************************************/
/* PUBLIC CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC VARIABLE DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION PROTOTYPES */
/******************************************************************************/
void __vector_19(void) __attribute__((signal));
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/


/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/
TWI_ErrorStatusTypeDef TWI_Init(TWI_HandleTypeDef *htwi)
{
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	if(htwi == NULL)
	{
		RET_enuErrorStatus = TWI_NULL_PTR_PASSED;
	}
	else
	{
		TWI_Handle = htwi;
		TWI->TWBR  = 4;
		TWI->TWSR  &= ~((uint8_t)3);
		TWI->TWSR  |= htwi->Init.Prescaler;
		TWI->TWAR  = (htwi->Init.OwnAddress << 1);
		TWI->TWCR  |= (1 << TWEA);
		TWI->TWAR  &= ~(1);
		TWI->TWAR  |= (htwi->Init.GeneralCallRecogantioon);
		TWI->TWCR  &= ~(1 << TWIE);
		TWI->TWCR  |= (htwi->Init.interrupt << TWIE);
	}
	return RET_enuErrorStatus;
}

TWI_ErrorStatusTypeDef TWI_Master_Transmit(TWI_HandleTypeDef *htwi,
					   uint16_t DevAddress, uint8_t *pData,
					   uint16_t Size, uint32_t Timeout)
{
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	TWI->TWCR = ((1 << TWINT) |(1 << TWEN ) | (1 << TWSTA)); 
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
	uint8_t Status = TWI->TWSR;
	switch(Status)
	{
		case TWI_START:
			TWI->TWDR = (DevAddress << 1);
			TWI->TWCR =((1 << TWINT) | (1 << TWEN)) ;
			while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
		break;

		default:
			RET_enuErrorStatus = TWI_FAILED;
		break;
	}
	Status = TWI->TWSR;
	switch(Status)
	{
		case TWI_MT_SLA_ACK:
			for(uint16_t data = 0 ; data < Size ; data++)
			{
				TWI->TWDR = pData[data];
				TWI->TWCR =((1 << TWINT) | (1 << TWEN)) ;
				while (!(TWI->TWCR & (1 << TWINT)) && Timeout--);
				if ((TWI->TWSR & TWI_MT_DATA_ACK) != TWI_MT_DATA_ACK)
				{
					RET_enuErrorStatus = TWI_FAILED;
				}
				if (Timeout == 0)
				{
					RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
				}
			}
		break;

		default:
			RET_enuErrorStatus = TWI_FAILED;
		break;
	}

	TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));
	TWI->TWCR = 0;
	if(Timeout == 0)
	{
		RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
	}
	return RET_enuErrorStatus;
}

TWI_ErrorStatusTypeDef TWI_Master_Receive(TWI_HandleTypeDef *htwi,
					  uint16_t DevAddress, uint8_t *pData,
					  uint16_t Size, uint32_t Timeout)
{ 
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	TWI->TWCR = ((1 << TWINT) |(1 << TWEN ) | (1 << TWSTA));
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
	uint8_t Status = TWI->TWSR;
	switch(Status)
	{
		case TWI_START:
		case 0x10:
		TWI->TWDR = ((DevAddress << 1) | 1);
		TWI->TWCR =((1 << TWINT) | (1 << TWEN)) ;
		while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
		break;

		default:
			RET_enuErrorStatus = TWI_FAILED;
		break;
	}
	Status = TWI->TWSR;
	switch(Status)
	{
		case TWI_MR_SLA_ACK :
			for(uint8_t data = 0 ; data < Size -1 ; data++)
			{
				TWI->TWCR = ((1 << TWINT) | (1 << TWEA) | (1 << TWEN));
				while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
				Status = TWI->TWSR;
				switch(Status)
				{
					case TWI_MR_DATA_ACK:
						pData[data] = TWI->TWDR;
						break;
					default:
						RET_enuErrorStatus = TWI_FAILED;
						break;
				}
			}
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
			while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
			pData[Size -1] = TWI->TWDR;
			break;
		default:
			RET_enuErrorStatus = TWI_FAILED;

	}
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));
	TWI->TWCR = 0;
	if(Timeout == 0)
	{
		RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
	}
	return RET_enuErrorStatus;
}

TWI_ErrorStatusTypeDef TWI_Slave_Transmit(TWI_HandleTypeDef *htwi,
					  uint8_t *pData, uint16_t Size,
					  uint32_t Timeout)
{
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	TWI->TWCR = ((1 << TWEN) | (1 << TWEA) | (1 << TWINT));
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
	uint8_t Status = TWI->TWSR;
	switch(Status)
	{
		case TWI_ST_SLA_ACK:
			 for(uint8_t data = 0 ; data < Size - 1 && ( RET_enuErrorStatus == TWI_OK) ; data++)
			 {
				TWI->TWDR = pData[data];
				TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWEA));
				    while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) {
					    if (Timeout == 0) {
						    RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
					    }
				    }
				Status = TWI->TWSR;
				switch(Status)
				{
					case  TWI_ST_DATA_ACK:
					case  TWI_ST_LAST_DATA:
						continue;
					break;

					default:
						RET_enuErrorStatus = TWI_FAILED;

				}
			 }
			TWI->TWDR = pData[Size - 1];
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
			while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) {
				if (Timeout == 0) {
					RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
				}
			}
			Status = TWI->TWSR;
			if(Status == TWI_ST_LAST_DATA)
			{
				TWI->TWCR = ((1 << TWINT) | (1 << TWEA));
			}
			 break;
		default:
			break;

	}
	return RET_enuErrorStatus;
}

TWI_ErrorStatusTypeDef TWI_Slave_Receive(TWI_HandleTypeDef *htwi,
					uint8_t *pData, uint16_t Size, 
					uint32_t Timeout)
{  
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	if(pData == NULL)
	{
		RET_enuErrorStatus = TWI_NULL_PTR_PASSED;
	}
	else
	{
		TWI->TWCR = ((1 << TWEN) | (1 << TWEA) | (1 << TWINT));
		while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
		uint8_t Status = TWI->TWSR;
		switch(Status)
		{
			case TWI_SR_SLA_ACK:
			case TWI_SR_ARB_LOST_SLA_ACK:
			case TWI_SR_GCALL_ACK:
				for(uint16_t data = 0 ; data < Size ; data++)
				{
					TWI->TWCR = ((1 << TWEN) | (1 << TWEA) | (1 << TWINT));
					while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
					Status = TWI->TWSR;
					switch(Status)
					{
						case TWI_SR_GCALL_DATA_ACK:
						case TWI_SR_DATA_ACK:
							pData[data] = TWI->TWDR;
						default:
							RET_enuErrorStatus = TWI_FAILED;
					}

				}
				TWI->TWCR = ((1 << TWEN) | (1 << TWEA) | (1 << TWINT));
			break;

			default:
				RET_enuErrorStatus = TWI_FAILED;
			break;
		}
		while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
		Status = TWI->TWSR;
		switch(Status)
		{
			case 0xA0:
				TWI->TWCR = ((1 << TWINT) | (1 << TWEA));
			break;
			default:
				RET_enuErrorStatus = TWI_FAILED;
			break;
		}
		if(Timeout == 0)
		{
			RET_enuErrorStatus = TWI_TIMEOUT_ERROR;
		}
	}
	return RET_enuErrorStatus;
}
TWI_ErrorStatusTypeDef TWI_Mem_Write(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
                                   uint16_t MemAddress, uint8_t byte, uint32_t Timeout) {
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	uint8_t LSB = (MemAddress &0xFF);
	uint8_t MSB = (MemAddress >> 8);
	uint8_t pData[2] = {LSB};
	TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);

	TWI->TWDR = (DevAddress << 1);
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);

	for(uint8_t data = 0 ; data < 1 ; data++)
	{
		TWI->TWDR = pData[data];
		TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
		while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);
	}
	TWI->TWDR = byte;
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
	while(!(TWI->TWCR & (1 << TWINT)) && Timeout--);

	TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));
	return RET_enuErrorStatus;
}


TWI_ErrorStatusTypeDef TWI_Mem_Read(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
uint16_t MemAddress, uint8_t *add, uint32_t Timeout) {
	TWI_ErrorStatusTypeDef RET_enuErrorStatus = TWI_OK;
	

	uint8_t LSB = (MemAddress & 0xFF);

	TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) {
	}

	TWI->TWDR = (DevAddress << 1);
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) {
	}

	TWI->TWDR = LSB; 
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) {
	}


	for(uint32_t i = 0 ; i < 10000 ; i++);
		
	TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN));
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--);
	TWI->TWDR = (DevAddress << 1) | 1;
	TWI->TWCR = ((1 << TWINT) | (1 << TWEN));
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) ;

	TWI->TWCR = ((1 << TWINT) | (1 << TWEN)); 
	while (!(TWI->TWCR & (1 << TWINT)) && Timeout--) ;
	
	*add = TWI->TWDR; 

	TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));

	return RET_enuErrorStatus;
}

TWI_ErrorStatusTypeDef TWI_Mem_Write_IT(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
				     uint16_t MemAddress, uint8_t byte)
{
	if(TWI_Busy != 1)
	{

		EEPROM_Write.data = byte;
		EEPROM_Write.DevAddress = DevAddress;
		EEPROM_Write.MemAddress = MemAddress;
		TWI_Busy= 1;
		service = WRITE_EEPROM_DATA;
		SREG |= (1 << 7);
		EEPROM_Write.cnt = 0;
		TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE));
		return 1;
	}
	return 0;
}
TWI_ErrorStatusTypeDef TWI_Mem_Read_IT(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
					uint16_t MemAddress, uint8_t *add)
{
		if(TWI_Busy != 1)
		{
			EEPROM_Write.data = ((uint8_t)add);
			EEPROM_Write.DevAddress = DevAddress;
			EEPROM_Write.MemAddress = MemAddress;
			TWI_Busy= 1;
			service = READ_EEPROM_DATA;
			SREG |= (1 << 7);
			EEPROM_Write.cnt = 0;
			TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE));
		return 1;
		}
		return 0;
}
void __vector_19(void)
{
	uint8_t status = TWI->TWSR;
	if(service == WRITE_EEPROM_DATA)
	{	
		if(status == TWI_START)
		{
			TWI->TWDR = (EEPROM_Write.DevAddress << 1);
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MT_SLA_ACK)
		{
			TWI->TWDR = EEPROM_Write.MemAddress;
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MT_DATA_ACK && EEPROM_Write.cnt == 0)
		{
			TWI->TWDR = EEPROM_Write.data;
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
			EEPROM_Write.cnt = 1;
		}
		else if(status == TWI_MT_DATA_ACK && EEPROM_Write.cnt == 1)
		{
			TWI_Busy = 0;
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));
		}
	}
	else if(service == READ_EEPROM_DATA)
	{
		if(status == TWI_START)
		{
			TWI->TWDR = (EEPROM_Write.DevAddress << 1);
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MT_SLA_ACK)
		{
			TWI->TWDR = EEPROM_Write.MemAddress;
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MT_DATA_ACK)
		{
			TWI->TWCR = ((1 << TWINT) | (1 << TWSTA) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_RESTART)	
		{
			TWI->TWDR = ((EEPROM_Write.DevAddress << 1) | 1);
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MR_SLA_ACK)
		{
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
		}
		else if(status == TWI_MR_DATA_NACK)
		{
			*((uint8_t*)EEPROM_Write.data) = TWI->TWDR;
			TWI_Busy = 0;
			TWI->TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWSTO));

		}
	}
}


/******************************************************************************/