/*
 * os-api.h
 *
 */

#ifndef OS_API_H_
#define OS_API_H_


#include "freeRTOS.h"
#include "semphr.h"
#include "task.h"


#define  OSA_WAIT_FOREVER  0xFFFFFFFFU

typedef SemaphoreHandle_t sem_t;
typedef SemaphoreHandle_t pthread_mutex_t;
typedef TaskHandle_t      pthread_t;


int sem_init(sem_t *pSem, int shared, uint32_t initValue);
int sem_destroy(sem_t *pSem);
int sem_post(sem_t *pSem);
int sem_timedwait(sem_t *pSem, uint32_t timeout);

int pthread_mutex_init(pthread_mutex_t *pMutex);
int pthread_mutex_destroy(pthread_mutex_t *pMutex);
int pthread_mutex_lock(pthread_mutex_t *pMutex);
int pthread_mutex_unlock(pthread_mutex_t *pMutex);



#endif /* OS_API_H_ */
