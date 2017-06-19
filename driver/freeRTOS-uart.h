/*
 * freeRTOS-uart.h
 *
 */

#ifndef FREERTOS_UART_H_
#define FREERTOS_UART_H_

#include "freeRTOS.h"
#include <stdint.h>

int uart_open( unsigned int ulWantedBaud, unsigned int uxQueueLength );
int uart_read (uint8_t *buffer, int size, uint32_t timeout_ms);
int uart_write(uint8_t *buffer, int size);
void uart_close( int xPort );


#endif /* FREERTOS_UART_H_ */
