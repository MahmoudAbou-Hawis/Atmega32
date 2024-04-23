#include "queue.h"



Queue_enuErrors_t QUEUE_enuInit(queue * pQueue)
{
    Queue_enuErrors_t RET_enuErrorStatus = QUEUE_enuProcessDone;
    
    /*check the if the pointer not equal to null*/ 
    if(pQueue == NULL)
    {
        RET_enuErrorStatus = QUEUE_enuNullPtr;
    }
    else
    {
        /*initialize the queue*/
        pQueue->u8Front = pQueue->u8Size = 0;
        pQueue->u8Rear = QUEUE_SIZE - 1;
    }
    return RET_enuErrorStatus;
}


Queue_enuErrors_t QUEUE_enuPush(queue * pQueue,uint8_t u8Value)
{
    Queue_enuErrors_t RET_enuErrorStatus = QUEUE_enuProcessDone;
    
    if(pQueue == NULL)
    {
        RET_enuErrorStatus = QUEUE_enuNullPtr;
    }
    else if(pQueue ->u8Size == QUEUE_SIZE)
    {
        RET_enuErrorStatus = QUEUE_enuFull;
    }
    else
    {
        pQueue->u8Rear = (pQueue->u8Rear + 1) % QUEUE_SIZE;
        pQueue->array[pQueue->u8Rear] = u8Value;
        pQueue->u8Size++;
    }

    return RET_enuErrorStatus;
}


Queue_enuErrors_t QUEUE_enuPop(queue * pQueue , uint8_t * pVal)
{
    Queue_enuErrors_t RET_enuErrorStatus = QUEUE_enuProcessDone;
    if(pQueue == NULL || pVal == NULL)
    {
        RET_enuErrorStatus = QUEUE_enuNullPtr;
    }
    else if(pQueue ->u8Size == 0)
    {
        RET_enuErrorStatus = QUEUE_enuEmpty;
    }
    else
    {
        *pVal = pQueue->array[pQueue->u8Front];
        pQueue->u8Front = (pQueue->u8Front + 1) % QUEUE_SIZE;
        pQueue->u8Size--;
    }
    return RET_enuErrorStatus;
}


Queue_enuErrors_t QUEUE_enuGetRear(queue * pQueue, uint8_t * pVal)
{
    Queue_enuErrors_t RET_enuErrorStatus = QUEUE_enuProcessDone;
    if(pQueue == NULL || pVal == NULL)
    {
        RET_enuErrorStatus = QUEUE_enuNullPtr;
    }
    else if(pQueue->u8Size == 0)
    {
        RET_enuErrorStatus = QUEUE_enuEmpty;
    }
    else
    {
        *pVal = pQueue->array[pQueue->u8Rear];
    }
    return RET_enuErrorStatus;
}