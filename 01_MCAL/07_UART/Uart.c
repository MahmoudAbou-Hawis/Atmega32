/******************************************************************************/
/**
 * @file Uart.c
 * @brief Uart protocol implemention in c language
 *
 * @par Project Name
 * Uart Project
 *
 * @par Code Language
 * C
 *
 * @par Description
 * You can Transmit and receive with Uart protocol
 *
 * @par Author
 * Mahmoud Abou-Hawis
 *
 */
/******************************************************************************/

/******************************************************************************/
/* INCLUDES */
/******************************************************************************/

#include "Uart.h"
/******************************************************************************/

/******************************************************************************/
/* PRIVATE DEFINES */
/******************************************************************************/
#define UART0_BASE     (0x29)
#define TR_UART0       ((tstUartMemRegs *)UART0_BASE)
#define UART_HND       ((tstUartHandle *)(pvUartHnd))
#define   UCSRC       (*(unsigned volatile char*)0x40)
#define BIT_UDRE0      (5)
#define BIT_RXC0       (7)
#define UCSZn2         (2)
#define USBSn          (3)
#define UPMn0          (4)
#define UCSZn0         (1)
#define NUM_OF_HANDLES (1)
/******************************************************************************/

/******************************************************************************/
/* PRIVATE MACROS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE ENUMS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE TYPES */
/******************************************************************************/

typedef struct
{
    volatile uint8_t u16Ubrr;
    volatile uint8_t u8UcsrB;
    volatile uint8_t u8UcsrA;
    volatile uint8_t u8Udr;
    volatile uint8_t reserved[19];
    volatile uint8_t u8UcsrC;
} tstUartMemRegs;


typedef struct
{
    tstUartMemRegs* pstUartMemRegs;

    void * pvSendBuffer;
    uint16_t u16SendBufferLength;
    void (*pfnSendBufferCallback)(void *, uint16_t);
    int SendBufferindex;
    void * pvReciveBuffer;
    uint16_t u16ReciveBufferLength;
    void (*pfnReciveBufferCallback)(void *, uint16_t);
    int ReciveBufferindex;
    bool bRecivedFlag;

} tstUartHandle;


/******************************************************************************/

/******************************************************************************/
/* PRIVATE CONSTANT DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PRIVATE VARIABLE DEFINITIONS */
/******************************************************************************/

static tstUartHandle astHandles[NUM_OF_HANDLES] =
{
    {
        .pstUartMemRegs = TR_UART0
    }
};

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
void  vTransmitByte(void *pvUartHnd, uint8_t u8Byte, uint16_t u16TimeOut);
uint8_t u8ReceiveByte(void *pvUartHnd, uint16_t u16TimeOut);
/******************************************************************************/

/******************************************************************************/
/* PRIVATE FUNCTION DEFINITIONS */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* PUBLIC FUNCTION DEFINITIONS */
/******************************************************************************/

void *Uart_pvInit(Uart_tstInitConfig *UartInit)
{
    if (UartInit->u8UartIdx > NUM_OF_HANDLES)
    {
        return NULL;
    }
    astHandles[UartInit->u8UartIdx].pstUartMemRegs->u8UcsrB = ((UartInit->u8InterruptType | UartInit->u8Direction) | (UartInit->enmCharSize & 1 << UCSZn2));
    astHandles[UartInit->u8UartIdx].pstUartMemRegs->u16Ubrr=((UartInit->u32SystemClock / (16 * (uint32_t)UartInit->u32BaudRate)) - 1);
    uint8_t temp = (UartInit->enmCharSize != UART_SIZE_9) ? (UartInit->enmCharSize << UCSZn0) : (UartInit->enmCharSize - 1 << UCSZn0);
    temp |=  ((UartInit->enmStopBits << USBSn) | (UartInit->enmParityType << UPMn0));
    astHandles[UartInit->u8UartIdx].pstUartMemRegs->u8UcsrC |= ((1 << 7) | temp);
    astHandles[UartInit->u8UartIdx].bRecivedFlag = 0;
    return (void *)&astHandles[UartInit->u8UartIdx];
}

void vTransmitByte(void *pvUartHnd, uint8_t u8Byte, uint16_t u16TimeOut)
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    uint16_t u16Counter = 0;
    while ((!(UART_HND->pstUartMemRegs->u8UcsrA & (1 << BIT_UDRE0))) && (u16Counter < u16TimeOut))
    {
        u16Counter++;
    }

    if (!u16TimeOut)
    {
        return;
    }

    UART_HND->pstUartMemRegs->u8Udr = u8Byte;
}

uint8_t u8ReceiveByte(void *pvUartHnd, uint16_t u16TimeOut)
{
    if (pvUartHnd == NULL)
    {
        return 0;
    }

    uint16_t u16Counter = 0;
    while ((!(UART_HND->pstUartMemRegs->u8UcsrA & (1 << BIT_RXC0))) && u16Counter < u16TimeOut)
    {
        u16Counter++;
    }

    if (u16Counter == u16TimeOut)
    {
        UART_HND->bRecivedFlag = false;
    }
    else
    {
        UART_HND->bRecivedFlag = true;
    }
    return UART_HND->pstUartMemRegs->u8Udr;
}

void Uart_vTransmitBuff(void *pvUartHnd, void *pvBuff, uint16_t u16Length, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    for (int idx = 0; idx < u16Length; idx++)
    {
        while (!(UART_HND->pstUartMemRegs->u8UcsrA & (1 << BIT_UDRE0)));
        UART_HND->pstUartMemRegs->u8Udr = ((uint8_t *)pvBuff)[idx];
    }

    if (pfnCallback != NULL)
    {
        (*pfnCallback)(pvBuff, u16Length);
    }
}

void Uart_vReceiveBuff(void *pvUartHnd, void *pvBuff, uint16_t u16Length, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    for (int idx = 0; idx < u16Length; idx++)
    {
        while (!(UART_HND->pstUartMemRegs->u8UcsrA & (1 << BIT_RXC0)));
        ((uint8_t *)pvBuff)[idx] = UART_HND->pstUartMemRegs->u8Udr;
    }

    if (pfnCallback != NULL)
    {
        (*pfnCallback)(pvBuff, u16Length);
    }
}

void Uart_vTransmitBuffTimeout(void *pvUartHnd, void *pvBuff, uint16_t u16Length, uint16_t u16Timeout, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    for (int idx = 0; idx < u16Length; idx++)
    {
        vTransmitByte(pvUartHnd, ((uint8_t *)pvBuff)[idx], u16Timeout);
    }
    if (pfnCallback != NULL)
    {
        (*pfnCallback)(pvBuff, u16Length);
    }
}

void Uart_vReceiveBuffTimeout(void *pvUartHnd, void *pvBuff, uint16_t u16Length, uint16_t u16Timeout, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    for (int idx = 0; idx < u16Length; idx++)
    {
        uint8_t Data = u8ReceiveByte(pvUartHnd, u16Timeout);
        if (UART_HND->bRecivedFlag)
        {
            ((uint8_t *)pvBuff)[idx] = Data;
        }
    }
    if (pfnCallback != NULL)
    {
        (*pfnCallback)(pvBuff, u16Length);
    }
}

void Uart_vTransmitBuffInterrupt(void *pvUartHnd, void *pvBuff, uint16_t u16Length, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }
    UART_HND->pvSendBuffer = pvBuff;
    UART_HND->SendBufferindex = 1;
    UART_HND->pfnSendBufferCallback = pfnCallback;
    UART_HND->u16SendBufferLength = u16Length;
    UART_HND->pstUartMemRegs->u8Udr = ((uint8_t *)pvBuff)[0];
}

void Uart_vReceiveBuffInterrupt(void *pvUartHnd, void *pvBuff, uint16_t u16Length, void (*pfnCallback)(void *, uint16_t))
{
    if (pvUartHnd == NULL)
    {
        return;
    }

    UART_HND->pvReciveBuffer = pvBuff;
    UART_HND->ReciveBufferindex = 0;
    UART_HND->pfnReciveBufferCallback = pfnCallback;
    UART_HND->u16ReciveBufferLength = u16Length;
}

