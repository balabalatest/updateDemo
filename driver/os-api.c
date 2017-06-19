/*
 * os-api.c
 *
 */

#include "os-api.h"
#include "debug.h"


#if 0
/*! @brief Converts milliseconds to ticks*/
#define MSEC_TO_TICK(msec)  (((uint32_t)(msec)+500uL/(uint32_t)configTICK_RATE_HZ) \
                             *(uint32_t)configTICK_RATE_HZ/1000uL)
#define TICKS_TO_MSEC(tick) ((tick)*1000uL/(uint32_t)configTICK_RATE_HZ)
#endif

#define MSEC_TO_TICK  pdMS_TO_TICKS

int sem_init(sem_t *pSem, int shared, uint32_t initValue)
{
    assert(pSem);
    (void) shared;

    *pSem = xSemaphoreCreateCounting(0xFF, initValue);
    if (*pSem==NULL)
    {
        return -1; /* creating semaphore failed */
    }
    return 0;
}

int sem_destroy(sem_t *pSem)
{
    assert(pSem);
    assert(*pSem);

    vSemaphoreDelete(*pSem);
    return 0;
}

int sem_post(sem_t *pSem)
{
    assert(pSem);

    if (pdTRUE == xSemaphoreGive(*pSem))
    {
        return 0; /* sync object given */
    }
    else
    {
        return -1;
    }
}

int sem_timedwait(sem_t *pSem, uint32_t timeout)
{
    uint32_t timeoutTicks;
    assert(pSem);

    /* Convert timeout from millisecond to tick. */
    if (timeout == OSA_WAIT_FOREVER)
    {
        timeoutTicks = portMAX_DELAY;
    }
    else
    {
        timeoutTicks = MSEC_TO_TICK(timeout);
    }

    if (xSemaphoreTake(*pSem, timeoutTicks)==pdFALSE)
    {
        return -1; /* timeout */
    }
    else
    {
        return 0; /* semaphore taken */
    }
}

int pthread_mutex_init(pthread_mutex_t *pMutex)
{
    assert(pMutex);
    *pMutex = xSemaphoreCreateMutex();
    if (NULL == *pMutex)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
int pthread_mutex_destroy(pthread_mutex_t *pMutex)
{
    assert(pMutex);
    assert(*pMutex);

    vSemaphoreDelete(*pMutex);
    return 0;
}


int pthread_mutex_lock(pthread_mutex_t *pMutex)
{
    uint32_t timeoutTicks;

    assert(pMutex);

    /* If pMutex has been locked by current task, return error. */
    if (xSemaphoreGetMutexHolder(*pMutex) == xTaskGetCurrentTaskHandle())
    {
        return -1;
    }

    if (xSemaphoreTake(*pMutex, OSA_WAIT_FOREVER)==pdFALSE)
    {
        return -1; /* timeout */
    }
    else
    {
        return 0; /* semaphore taken */
    }
}
int pthread_mutex_unlock(pthread_mutex_t *pMutex)
{
    assert(pMutex);

    /* If pMutex is not locked by current task, return error. */
    if (xSemaphoreGetMutexHolder(*pMutex) != xTaskGetCurrentTaskHandle())
    {
        return -1;
    }

    if (xSemaphoreGive(*pMutex)==pdPASS)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
