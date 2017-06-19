/*******************************************************************************
 * Copyright (c) 2014, 2015 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander - initial API and implementation and/or initial documentation
 *    Ian Craggs - convert to FreeRTOS
 *******************************************************************************/

#include "MQTTFreeRTOS.h"

#include "socket_port.h"
#include "addr.h"
#include <errno.h>
#include <time.h>


int ThreadStart(Thread* thread, void (*fn)(void*), void* arg)
{
	int rc = 0;
	uint16_t usTaskStackSize = (configMINIMAL_STACK_SIZE * 5);
	UBaseType_t uxTaskPriority = uxTaskPriorityGet(NULL); /* set the priority as the same as the calling task*/

	rc = xTaskCreate(fn,	/* The function that implements the task. */
		"MQTTTask",			/* Just a text name for the task to aid debugging. */
		usTaskStackSize,	/* The stack size is defined in FreeRTOSIPConfig.h. */
		arg,				/* The task parameter, not used in this case. */
		uxTaskPriority,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
		&thread->task);		/* The task handle is not used. */

	return rc;
}


void MutexInit(Mutex* mutex)
{
	mutex->sem = xSemaphoreCreateMutex();
}

int MutexLock(Mutex* mutex)
{
	return xSemaphoreTake(mutex->sem, portMAX_DELAY);
}

int MutexUnlock(Mutex* mutex)
{
	return xSemaphoreGive(mutex->sem);
}


void TimerCountdownMS(Timer* timer, unsigned int timeout_ms)
{
	timer->xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */
	vTaskSetTimeOutState(&timer->xTimeOut); /* Record the time at which this function was entered. */
}


void TimerCountdown(Timer* timer, unsigned int timeout) 
{
	TimerCountdownMS(timer, timeout * 1000);
}


int TimerLeftMS(Timer* timer) 
{
	xTaskCheckForTimeOut(&timer->xTimeOut, &timer->xTicksToWait); /* updates xTicksToWait to the number left */
	return (timer->xTicksToWait < 0) ? 0 : (timer->xTicksToWait * portTICK_PERIOD_MS);
}


int TimerIsExpired(Timer* timer)
{
	return xTaskCheckForTimeOut(&timer->xTimeOut, &timer->xTicksToWait) == pdTRUE;
}


void TimerInit(Timer* timer)
{
	timer->xTicksToWait = 0;
	memset(&timer->xTimeOut, '\0', sizeof(timer->xTimeOut));
}

#if 1
int linux_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
  {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  Setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

  int bytes = 0;
  while (bytes < len)
  {
    int rc = Recv(n->my_socket, &buffer[bytes], (size_t)(len - bytes), 0);
    if (rc == -1)
    {
      if (errno != ENOTCONN && errno != ECONNRESET)
      {
        bytes = -1;
        break;
      }
    }
    else
      bytes += rc;
  }
  return bytes;
}

#else
int linux_read(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
  {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  Setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

  int bytes = 0;

  {
    int rc = Recv(n->my_socket, &buffer[bytes], (size_t)(len - bytes), 0);
    if (rc == -1)
    {
      if (errno != ENOTCONN && errno != ECONNRESET)
      {
        bytes = -1;
      }
    }
    else
      bytes += rc;
  }

  return bytes;
}
#endif

int linux_write(Network* n, unsigned char* buffer, int len, int timeout_ms)
{
  struct timeval tv;

  tv.tv_sec = 0;  /* 30 Secs Timeout */
  tv.tv_usec = timeout_ms * 1000;  // Not init'ing this can cause strange errors

  Setsockopt(n->my_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

  int bytes = 0;
  while (bytes < len)
  {
    int rc = Send(n->my_socket, &buffer[bytes], (size_t)(len - bytes), 0);
    if (rc == -1)
    {
      if (errno != ENOTCONN && errno != ECONNRESET)
      {
        bytes = -1;
        break;
      }
    }
    else
      bytes += rc;
  }
  return bytes;
}


void NetworkInit(Network* n)
{
  n->my_socket = 0;
  n->mqttread = linux_read;
  n->mqttwrite = linux_write;
}



void NetworkDisconnect(Network* n)
{
  Close(n->my_socket);
}

int NetworkConnect(Network* n, char* addr, int port)
{
  struct sockaddr_in address;
  struct hostent *hptr;
  int rc = -1;

#if 1
  if ((hptr = Gethostbyname(addr)) == NULL)
    goto exit;

  address.sin_addr = *((struct in_addr *)hptr->h_addr);
#else
  address.sin_addr.s_addr = inet_addr(addr);
#endif

  address.sin_family = AF_INET;
  address.sin_port = htons(port);

  if ((n->my_socket = Socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    goto exit;

  if ((rc = Connect(n->my_socket, (struct sockaddr*) &address, sizeof(address))) < 0)
  {
    Close(n->my_socket);
    goto exit;
  }

exit:
  return rc;
}
