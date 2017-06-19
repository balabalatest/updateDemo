/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "stm32f10x.h"
			

void led_init(void)
{
	GPIO_InitTypeDef gpioInitStrcut;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	gpioInitStrcut.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInitStrcut.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioInitStrcut);
}


void led_task(void *arg)
{
	led_init();

	while (1) {

    GPIO_WriteBit(GPIOC, GPIO_Pin_7, 0);
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, 0);
		vTaskDelay(pdMS_TO_TICKS(300));

    GPIO_WriteBit(GPIOC, GPIO_Pin_7, 1);
    GPIO_WriteBit(GPIOC, GPIO_Pin_8, 1);
		vTaskDelay(pdMS_TO_TICKS(300));

	}
}
