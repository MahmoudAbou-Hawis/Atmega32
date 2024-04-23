/******************************************************************************/
/**
 * @file TWI.h
 * @brief TWI (Two-Wire Interface) driver header for ATmega32
 *
 * @par Project Name
 * atmega32 MCAl
 *
 * @par Code Language
 * C
 *
 * @par Description
 * This header file provides function prototypes and relevant definitions for
 * interacting with the TWI (Two-Wire Interface) module on the ATmega32 microcontroller.
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef _TWI_H
#define _TWI_H
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
/**
 * @brief TWI error status enumeration
 */
typedef enum
{
    TWI_OK,                      /**< Operation completed successfully */
    TWI_FAILED,                  /**< Operation failed */
    TWI_NULL_PTR_PASSED,         /**< Null pointer passed to the function */
    TWI_TIMEOUT_ERROR
} TWI_ErrorStatusTypeDef;

/**
 * @brief Enumeration for TWI general call recognition options
 */
typedef enum
{
    TWI_GeneralCall_Disable = 0,    /**< General call recognition disabled */
    TWI_GeneralCall_Enable          /**< General call recognition enabled */
} TWI_GeneralCallOptions;

/**
 * @brief Enumeration for TWI interrupt options
 */
typedef enum
{
    TWI_Interrupt_Disable = 0,  /**< TWI interrupt disabled */
    TWI_Interrupt_Enable        /**< TWI interrupt enabled */
} TWI_InterruptOptions;

/**
 * @brief Enumeration for TWI prescaler types
 */
typedef enum
{
    TWI_PRESCALLER_1 = 0,   /**< Prescaler value of 1 */
    TWI_PRESCALLER_4,       /**< Prescaler value of 4 */
    TWI_PRESCALLER_16,      /**< Prescaler value of 16 */
    TWI_PRESCALLER_64       /**< Prescaler value of 64 */
} TWI_PrescallerTypeDef;
/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/
/**
 * @brief Structure for TWI initialization parameters
 */
typedef struct
{
    TWI_PrescallerTypeDef Prescaler;            /**< TWI prescaler value */
    uint32_t SCLFrequency;                      /**< TWI clock frequency */
    TWI_GeneralCallOptions GeneralCallRecogantioon; /**< General call recognition option */
    uint8_t OwnAddress;                         /**< Own address in TWI communication */
    TWI_InterruptOptions interrupt;             /**< TWI interrupt option */
} TWI_InitTypeDef;

/**
 * @brief Structure for TWI handle
 */
typedef struct  
{
    TWI_InitTypeDef Init;                       /**< TWI initialization parameters */
    uint8_t *pBuffPtr;                          /**< Pointer to TWI transfer buffer */
    uint16_t XferSize;                          /**< TWI transfer buffer size */
    volatile uint16_t XferCount;                /**< TWI transfer buffer counter */
    uint8_t *pBuffPtrR;                         /**< Pointer to TWI receive buffer */         
    uint16_t XResSize;                          /**< I2C receive buffer size */
    volatile uint16_t XResCount;                /**< TWI transfer buffer counter */
    void (*CallBack)(void);                     /**< Callback function pointer */
} TWI_HandleTypeDef;
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
 * @brief Initialize TWI module
 * @param htwi TWI handle pointer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Init(TWI_HandleTypeDef *htwi);

/**
 * @brief Transmit data as a master device over TWI
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Master_Transmit(TWI_HandleTypeDef *htwi,
					 uint16_t DevAddress, uint8_t *pData,
					 uint16_t Size, uint32_t Timeout);

/**
 * @brief Receive data as a master device over TWI
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Master_Receive(TWI_HandleTypeDef *htwi,
					 uint16_t DevAddress, uint8_t *pData,
					 uint16_t Size, uint32_t Timeout);

/**
 * @brief Transmit data as a slave device over TWI
 * @param htwi TWI handle pointer
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Slave_Transmit(TWI_HandleTypeDef *htwi,
					 uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief Receive data as a slave device over TWI
 * @param htwi TWI handle pointer
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Slave_Receive(TWI_HandleTypeDef *htwi, 
					 uint8_t *pData, uint16_t Size, uint32_t Timeout);

/**
 * @brief Write data to a memory device over TWI
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param MemAddress Memory address
 * @param MemAddSize Memory address size
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Mem_Write(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
				    uint16_t MemAddress, uint8_t byte, uint32_t Timeout);

/**
 * @brief Read data from a memory device over TWI
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param MemAddress Memory address
 * @param MemAddSize Memory address size
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @param Timeout Timeout value
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Mem_Read(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
				    uint16_t MemAddress, uint8_t *pData, uint32_t Timeout);

/**
 * @brief Transmit data as a master device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Master_Transmit_IT(TWI_HandleTypeDef *htwi, 
					      uint16_t DevAddress, uint8_t *pData, uint16_t Size);

/**
 * @brief Receive data as a master device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Master_Receive_IT(TWI_HandleTypeDef *hi2c, uint16_t DevAddress,
					    uint8_t *pData, uint16_t Size);

/**
 * @brief Transmit data as a slave device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Slave_Transmit_IT(TWI_HandleTypeDef *hi2c,
				             uint8_t *pData, uint16_t Size);

/**
 * @brief Receive data as a slave device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Slave_Receive_IT(TWI_HandleTypeDef *hi2c, 
					    uint8_t *pData, uint16_t Size);

/**
 * @brief Write data to a memory device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param MemAddress Memory address
 * @param MemAddSize Memory address size
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Mem_Write_IT(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
					uint16_t MemAddress, uint8_t byte);

/**
 * @brief Read data from a memory device over TWI with interrupt
 * @param htwi TWI handle pointer
 * @param DevAddress Device address
 * @param MemAddress Memory address
 * @param MemAddSize Memory address size
 * @param pData Pointer to data buffer
 * @param Size Size of data buffer
 * @return TWI error status
 */
TWI_ErrorStatusTypeDef TWI_Mem_Read_IT(TWI_HandleTypeDef *htwi, uint16_t DevAddress,
					uint16_t MemAddress, uint8_t *add);
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
#endif /* _TWI_H */
/******************************************************************************/
