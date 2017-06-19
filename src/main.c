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


extern void led_task(void *arg);
extern void adxl345(void *arg);
extern void net_task(void *arg);
extern void prvMQTTEchoTask(void *pvParameters);

void vPortSetupTimerInterrupt( void )
{
  SystemCoreClockUpdate();
  if (SysTick_Config(SystemCoreClock / configTICK_RATE_HZ))
  {
    printf("SysTick Config Error!\n");
    printf("System Halting...\n");

    /* Capture error */
    while (1);

  }
}

const static char *version = "FreeRTOS Esp8266 Network Demo-V1.00";

int main(void)
{
	debug_init();
	printf("\n%s\n", version);

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);
//  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x20000);

	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

  xTaskCreate(net_task, "Task-net", 1024, NULL, configMAX_PRIORITIES-3, NULL);

//  xTaskCreate(led_task, "Task-led4", 1024, NULL, configMAX_PRIORITIES-4, NULL);
//  xTaskCreate(adxl345, "sensors task", 1024, NULL, configMAX_PRIORITIES-4, NULL);


	vTaskStartScheduler();

	printf("Never here!\n");

	for(;;);
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, char *pcTaskName )
{
  ( void ) pcTaskName;
  ( void ) pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */

	printf("Error FreeRTOS Stack Over Flow...\n");
  taskDISABLE_INTERRUPTS();
  for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c, heap_2.c or heap_4.c are used, then the
  size of the heap available to pvPortMalloc() is defined by
  configTOTAL_HEAP_SIZE in FreeRTOSConfig.h, and the xPortGetFreeHeapSize()
  API function can be used to query the size of free heap space that remains
  (although it does not provide information on how the remaining heap might
  be fragmented). */
  taskDISABLE_INTERRUPTS();
  for( ;; );
}
/*-----------------------------------------------------------*/
