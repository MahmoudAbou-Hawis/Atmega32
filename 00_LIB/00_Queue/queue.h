#include "../Platform_Types.h"
#include "queue_CFG.h"

/**
 * @brief this contain all errors may occur while using the queue.
 */
typedef enum
{
    /**
     * @brief returned if the queue is empty.
     */
    QUEUE_enuEmpty,

    /**
     * @brief returned if the queue is full.
     */
    QUEUE_enuFull,

    /**
     * @brief returned if the queue is null or the value is null.
     */
    QUEUE_enuNullPtr,

    /**
     * @brief returned if the fuction do its functionality.
     */
    QUEUE_enuProcessDone

} Queue_enuErrors_t;

/**
 * @brief this is the queue data structure.
 */
typedef struct
{
    /**
     * @brief represent the last element in the queue.
     */
    uint8_t u8Rear;

    /**
     * @brief represent the first element in the queue.
     */
    uint8_t u8Front;

    /**
     * @brief represent the size of the queue.
     */
    uint8_t u8Size;

    /**
     * @brief array to safe the data.
     */
    uint8_t array[QUEUE_SIZE];

} queue;

/**
 * @brief used to initialize the queue.
 *
 * @param[in] pQueue pointer the queue which you want to init.
 *
 * @return Error status.
 *
 * @note you should call this function before calling any function.
 */
Queue_enuErrors_t QUEUE_enuInit(queue *pQueue);

/**
 * @brief used to push new element in the queue.
 *
 * @param[in] pQueue pointer the queue which you want to add the element in.
 *
 * @param[in] value  the value which you want to add in the queue.
 *
 * @return Error status.
 *
 * @note the queue should have places to add this element.
 */
Queue_enuErrors_t QUEUE_enuPush(queue *pQueue, uint8_t u8Value);

/**
 * @brief used to remove the element in the queue.
 *
 * @param[in] pQueue pointer the queue which you want to remove the element from.
 *
 * @param[in out] pVaL pointer which will contain the value of the removed element.
 *
 * @return Error status.
 *
 * @note the queue should have not empty.
 */
Queue_enuErrors_t QUEUE_enuPop(queue *pQueue, uint8_t *pVal);

/**
 * @brief used to get the last element in the queue.
 *
 * @param[in] pQueue pointer the queue which you want to get the element from.
 *
 * @param[in out] pVaL pointer which will contain the value of the last element.
 *
 * @return Error status.
 *
 * @note the queue should have not empty.
 */
Queue_enuErrors_t QUEUE_enuGetRear(queue *pQueue, uint8_t *pVal);