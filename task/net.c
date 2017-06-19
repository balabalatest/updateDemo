/*
 * net.c
 *
 */
/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "addr.h"

#include "freeRTOS.h"
#include "debug.h"

#include "socket_port.h"

#include "stm32f10x.h"

/* Here we include the header file for the application(s) we use in
 * our project as defined in the config/<board-name>/defconfig file
 */


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CONFIG_WIFI_SSID      "CMCC-IOT"
#define CONFIG_WIFI_PASSWORD  "IOT@Chinamobile"


/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int netsetup(void)
{

  int ret;
  int timeout;

  ret = lesp_initialize();
  if (ret != OK)
    {
      perror("ERROR: failed to init network\n");
      return ret;
    }

#if 1
  ret = lesp_soft_reset();
  if (ret != OK)
    {
      perror("ERROR: failed to reset\n");
      return ret;
    }

  timeout = 5;
  ret = lesp_ap_connect(CONFIG_WIFI_SSID, \
                        CONFIG_WIFI_PASSWORD, \
                        timeout);
  if (ret != OK)
    {
      perror("ERROR: failed to connect wifi\n");
      return ret;
    }
#endif

  return ret;
}


#if 1
char buffer[512];
void task_stat(void)
{
  memset(buffer, 0, sizeof(buffer));

  strcat((char *)buffer, "\nName\t\t\tState\tPriority\tStack\tNum\n" );

  strcat((char *)buffer, "--------------------------------------------------------\n");
  vTaskList((unsigned char *)(buffer + strlen(buffer)));
  printf("%s", buffer);
}
#endif


extern void prvMQTTEchoTask(void *pvParameters);

#define CONFIG_HTTP_DOWNLOAD_ADDRESS  "192.168.1.112/firmware.bin"

int net_task(int argc, char *argv[])
{
  int ret;

  lesp_global_init();

  ret = netsetup();
  if (ret != OK) {
    while (1);
  }

#if 1
extern int key2_pressed(void);
extern key2_init(void);

  key2_init();
  printf("\nPress SW2 to start Update\n\n");

  while (!key2_pressed())
    ;

  if (http_download(CONFIG_HTTP_DOWNLOAD_ADDRESS)) {
    printf("Going to Reset System...\n");
    NVIC_SystemReset();
    while (1);
  }
  printf("Http Update Failed\n");
#endif


#if 0
  xTaskCreate(prvMQTTEchoTask,  /* The function that implements the task. */
      "Task-mqttEcho",      /* Just a text name for the task to aid debugging. */
      1024, /* The stack size is defined in FreeRTOSIPConfig.h. */
      NULL,   /* The task parameter, not used in this case. */
      configMAX_PRIORITIES-2,   /* The priority assigned to the task is defined in FreeRTOSConfig.h. */
      NULL);        /* The task handle is not used. */
#endif

//  xTaskCreate(tcpEcho, "Task-tcpEcho", 1024, NULL, configMAX_PRIORITIES, NULL);

  for (;;) {
//    task_stat();
    vTaskDelay(pdMS_TO_TICKS(10*1000));

  }

#if 0
  ret = Close(sockfd);
  if (ret != OK)
    {
      perror("ERROR: failed to close socket\n");
      exit(1);
    }

  printf("Task end\n");
#endif
  return ret;
}
