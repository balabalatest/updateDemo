/*
 * key.c
 *
 */

#include "stm32f10x.h"
#include "freeRTOS.h"
#include "stm32f10x_gpio.h"

void key2_init(void)
{
  GPIO_InitTypeDef gpioInitStrcut;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  gpioInitStrcut.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioInitStrcut.GPIO_Pin = GPIO_Pin_11;
  gpioInitStrcut.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &gpioInitStrcut);
}

int key2_pressed(void)
{
  if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_RESET) {
    vTaskDelay(pdMS_TO_TICKS(10));
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) == Bit_RESET) {
      return 1;
    }
  }
  return 0;
}
