/*
 * freeRTOS-uart.c
 *
 */


/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved
    1 tab == 4 spaces!
*/

/*
  BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER FOR UART0.
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/* Library includes. */
#include "stm32f10x_usart.h"

/* Demo application includes. */
#include "freeRTOS-uart.h"
/*-----------------------------------------------------------*/

/* Misc defines. */
#define serINVALID_QUEUE        ( ( QueueHandle_t ) 0 )
#define serNO_BLOCK           ( ( TickType_t ) 0 )

/*-----------------------------------------------------------*/

/* The queue used to hold received characters. */
static QueueHandle_t xRxedChars;
static QueueHandle_t xCharsForTx;

/*-----------------------------------------------------------*/

#define UART_FD  0

/*
 * See the serial2.h header file.
 */
int uart_open( unsigned int ulWantedBaud, unsigned int uxQueueLength )
{
int xReturn = UART_FD;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
GPIO_InitTypeDef GPIO_InitStructure;

  /* Create the queues used to hold Rx/Tx characters. */
  xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
  xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );

  /* If the queue/semaphore was created correctly then setup the serial port
  hardware. */
  if( ( xRxedChars != serINVALID_QUEUE ) && ( xCharsForTx != serINVALID_QUEUE ) )
  {
    /* Enable USART2 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART2 Rx (PA3) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    /* Configure USART2 Tx (PA2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    USART_InitStructure.USART_BaudRate = ulWantedBaud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init( USART2, &USART_InitStructure );

    USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_KERNEL_INTERRUPT_PRIORITY;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    USART_Cmd( USART2, ENABLE );
  }
  else
  {
    xReturn = -1;
  }

  /* This demo file only supports a single port but we have to return
  something to comply with the standard demo header file. */
  return xReturn;
}
/*-----------------------------------------------------------*/

int uart_read(uint8_t *buffer, int size, uint32_t timeout_ms)
{
  int recvLen = 0;

  TickType_t xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */

  do
  {
    if( xQueueReceive( xRxedChars, buffer + recvLen, xTicksToWait ) )
    {
      if ( ++recvLen >= size )
        break;

      xTicksToWait = 1;
    }
    else
    {
      break;
    }

  } while (1);

  return recvLen;
}

/*-----------------------------------------------------------*/
#define  OSA_WAIT_FOREVER  0xFFFFFFFFU

int uart_write(uint8_t *buffer, int size)
{
  int sendLen = 0;

  do {
    if( xQueueSend( xCharsForTx, buffer + sendLen, OSA_WAIT_FOREVER ) == pdPASS )
    {
      if ( ++sendLen >= size )
        break;
    }
    else
    {
      return sendLen;
    }
  } while (1);

  USART_ITConfig( USART2, USART_IT_TXE, ENABLE );

  return sendLen;
}

/*-----------------------------------------------------------*/

void uart_close( int xPort )
{
  /* Not supported as not required by the demo application. */
}
/*-----------------------------------------------------------*/

void USART2_IRQHandler( void )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
char cChar;

  if( USART_GetITStatus( USART2, USART_IT_TXE ) != RESET )
  {
    /* The interrupt was caused by the THR becoming empty.  Are there any
    more characters to transmit? */
    if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xHigherPriorityTaskWoken ) == pdTRUE )
    {
      /* A character was retrieved from the queue so can be sent to the
      THR now. */
      USART_SendData( USART2, cChar );
    }
    else
    {
      USART_ITConfig( USART2, USART_IT_TXE, DISABLE );
    }
  }

  if( USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET )
  {
    cChar = USART_ReceiveData( USART2 );
    xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
  }

  /* TODO other fault flags */

  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}






