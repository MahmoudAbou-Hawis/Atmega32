/******************************************************************************/
/**
 * @file ADC.h
 * @brief Atmega32P ADC implemention in C language
 *
 * @par Project Name
 * ADC Driver
 *
 * @par Code Language
 * C
 *
 * @par Description
 * You Can get the digital value of analog signal by using this driver
 * 
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/

/******************************************************************************/
/* MULTIPLE INCLUSION GUARD */
/******************************************************************************/
#ifndef ADC_H
#define ADC_H
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

/*----------------------------------------------------------------------------*/
/** @brief Represents possible Voltage reference choices
 */
/*----------------------------------------------------------------------------*/

typedef enum 
{
    /** @brief AREF, internal V REF turned off
     */
    ADC_ARED = 0,

    /** @brief AV CC with external capacitor at AREF pin
     */
    ADC_Avcc = 1,

    /** @brief Internal 2.56V voltage reference with external capacitor at AREF pin
     */
    ADC_INTERNAL_1V = 3


} ADC_tenmVoltageReference;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** @brief Represents possible Channels choices from ADC0 to ADC7
 */
/*----------------------------------------------------------------------------*/

typedef enum
{
    /** @brief Selected Channel is ADC0
     */
    ADC0 = 0,

     /** @brief Selected Channel is ADC1
     */
    ADC1 = 1,

     /** @brief Selected Channel is ADC2
     */
    ADC2 = 2,

     /** @brief Selected Channel is ADC3
     */
    ADC3 = 3,

    /** @brief Selected Channel is ADC4
     */
    ADC4 = 4,

    /** @brief Selected Channel is ADC5
     */
    ADC5 = 5,

    /** @brief Selected Channel is ADC6
     */
    ADC6 = 6,
    
    /** @brief Selected Channel is ADC7
     */
    ADC7 = 7

} ADC_tenmAnolagChannals;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** @brief Represents Interrupt choices
 */
/*----------------------------------------------------------------------------*/

typedef enum
{
    /** @brief Disable ADC interrupt
     */
    ADC_INTERRUPT_NONE = 0,

    /** @brief Enable ADC interrupt
     */
    ADC_INTERRUPT_EN

} ADC_tenmInterruptType;
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/** @brief Represents ADC Prescaler Selections
 */
/*----------------------------------------------------------------------------*/

typedef enum
{
    /** @brief Frequancy of the resultant conversion equal  8000 KHz
     */
    ADC_Division_Factor_2   = 1,

    /** @brief Frequancy of the resultant conversion equal  4000 KHz
     */
    ADC_Division_Factor_4   = 2, 

    /** @brief Frequancy of the resultant conversion equal  2000 KHz
     */
    ADC_Division_Factor_8   = 3,

    /** @brief Frequancy of the resultant conversion equal  1000 KHz
     */
    ADC_Division_Factor_16  = 4,

    /** @brief Frequancy of the resultant conversion equal  500 KHz
     */
    ADC_Division_Factor_32  = 5,

    /** @brief Frequancy of the resultant conversion equal  250 KHz
     */
    ADC_Division_Factor_64  = 6,

    /** @brief Frequancy of the resultant conversion equal  125 KHz
     */
    ADC_Division_Factor_128 = 7

} ADC_PrescalerSelections;

typedef enum
{
	ADC_OK,
	ADC_NOT_OK,
	ADC_NULL_PTR_PASSED,
} ADC_enuErrorStatus_t;

/******************************************************************************/

/******************************************************************************/
/* PUBLIC TYPES */
/******************************************************************************/


/*----------------------------------------------------------------------------*/
/** @brief Represents the initial configuration for the ADC 
 */
/*----------------------------------------------------------------------------*/

typedef struct
{
    /** @brief The identifier of the Voltage Reference 
     * 
     * It could be one of:
     *    - ARED
     *    - Avcc
     *    - internal_1V
     */
    ADC_tenmVoltageReference enmVoltageReference;

    /** @brief The identifier of the InterruptType
     * 
     * It could be one of:
     *    - INTERRUPT_NONE
     *    - INTERRUPT_EN
     */
	ADC_tenmInterruptType enmInterruptType;

    /** @brief The identifier of the PrescalerSelections
     * 
     * It could be one of:
     *    - Division_Factor_2
     *    - Division_Factor_4
     *    - .
     *    - .
     *    - .
     *    - Division_Factor_128
     */

	ADC_PrescalerSelections enmPrescalerSelections;

} ADC_tstInitConfig;

typedef struct
{
	ADC_tenmAnolagChannals * Channels;
	uint8_t ChannelsNumber;
	uint16_t * Result;

} ADC_strChain_t;

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
/** @brief Initialize ADC 
 * 
 * @param[in] pstADCInit  A pointer to the configuration structure of the ADC
 *                        peripheral
 * @return Error status
 */
/*----------------------------------------------------------------------------*/
ADC_enuErrorStatus_t ADC_vInit(ADC_tstInitConfig* pstADCInit);

/*----------------------------------------------------------------------------*/
/**
 * @brief Starts an ADC conversion synchronously for a single channel.
 *
 * This function initiates an analog-to-digital conversion synchronously for
 * the specified channel (`Copy_u8Channel_Id`). It blocks the calling code until
 * the conversion is complete and the result is stored in the provided memory
 * location pointed to by `Add_pu16ADCRead`.
 *
 * @param Copy_u8Channel_Id The ID of the analog channel to convert. Refer to
 *                           the ADC documentation for valid channel IDs.
 * @param Add_pu16ADCRead  Pointer to a memory location where the 16-bit ADC
 *                           conversion result will be stored.
 *
 * @return ADC_enuErrorStatus_t An enumeration value indicating the status of
 *                              the operation:
 *                              - ADC_enuSUCCESS: Conversion successful.
 */
/*----------------------------------------------------------------------------*/
ADC_enuErrorStatus_t ADC_enuStartConversionSync  (ADC_tenmAnolagChannals Copy_u8Channel_Id
						, uint16_t * Add_pu16ADCRead);
/**
 * @brief Starts an ADC conversion asynchronously for a chain of channels.
 *
 * This function initiates an analog-to-digital conversion asynchronously for a
 * chain of channels defined in the provided `Copy_strChain` structure. The
 * calling code is not blocked and can continue execution while the ADC performs
 * the conversions. The specific format and contents of the `ADC_strChain_t`
 * structure depend on the implementation and should be documented by the ADC
 * library or vendor.
 *
 * @param Copy_strChain Pointer to a structure containing the chain of channels
 *                      to be converted. Refer to the ADC documentation for the
 *                      expected format of the `ADC_strChain_t` structure.
 *
 * @return ADC_enuErrorStatus_t An enumeration value indicating the status of
 *                              the operation:
 *                              - ADC_enuSUCCESS: Conversion successful.
 */
ADC_enuErrorStatus_t ADC_enuStartConversionAsync (ADC_strChain_t *Copy_strChain);


/**
 * @brief Registers a callback function for asynchronous ADC conversions.
 *
 * This function sets a callback function (`ADC_Callback`) to be invoked upon
 * completion of an asynchronous ADC conversion. The callback function is
 * typically used to handle the converted data or perform further actions after
 * the conversion is finished.
 *
 * @param ADC_Callback Pointer to a function that will be called when an
 *                      asynchronous ADC conversion is complete. The function
 *                      signature should be `void(*ADC_Callback)(void)`.
 *
 * @return ADC_enuErrorStatus_t An enumeration value indicating the status of
 *                              the operation:
 *                              - ADC_enuSUCCESS: Callback function set successfully.
 */
ADC_enuErrorStatus_t ADC_enuSetADCCallBack       (void(*ADC_Callback)(void));



/*----------------------------------------------------------------------------*/



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
#endif  
/******************************************************************************/