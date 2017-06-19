/*
 * esp8266.c
 */

#include <stdio.h>
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#include "FreeRTOS.h"
#include "task.h"

#if 0

void lesp_soft_reset(void)
{

  sleep(1);

  /* Send reset */

  lesp_send_cmd("AT+RST\r\n");

  /* Leave time to reset */

  sleep(1);

  lesp_flush();
}


#define ESP8266_BUFSIZE    1024

#define WIFI_SSID     "CMCC-IOT"
#define WIFI_PASSWD   "IOT@Chinamobile"

#define SERVER_IP      "183.230.40.39"
#define SERVER_PORT    6002

int esp8266_buf_init(void)
{

	return 0;
}

#define BUF_CMD_LEN    256
static uint8_t buffer[BUF_CMD_LEN];

#if 0
int esp8266_send_at(char *at, char *want, int timeout_ms)
{
  TimeOut_t xTimeOut;
  TickType_t xTicksToWait = timeout_ms / portTICK_PERIOD_MS; /* convert milliseconds to ticks */
  int recvLen = 0;

  memset(buffer, 0, sizeof(buffer));

  esp_write((uint8_t *)at, strlen(at), 10);
  fifo_flush(esp8266_io.rx);

  vTaskDelay(100 / portTICK_PERIOD_MS);

  vTaskSetTimeOutState(&xTimeOut); /* Record the time at which this function was entered. */

  do
  {
    int rc = 0;

    rc = esp_read(buffer + recvLen, sizeof(buffer) - recvLen, xTicksToWait);

    if (NULL != strstr(buffer, want))
      return 0;

    if (rc > 0)
      recvLen += rc;

  } while (xTaskCheckForTimeOut(&xTimeOut, &xTicksToWait) == pdFALSE);

  return -1;
}

#else

int esp8266_send_at(char *at, char *want, int timeout_ms)
{
  uint8_t *ptr = buffer;
  int size = BUF_CMD_LEN;

  esp_write(at, strlen(at));

  while (1) {

    int n = esp_read(ptr, BUF_CMD_LEN);

    if (n) {
      if (strstr(ptr, want) != NULL) {
        return 0;
      }

      ptr  += n;
      size -= n;
      if (size < 0) {
        return -1;
      }
    }

  }
}

#endif

int esp8266_connect_ap(const char *ssid, const char *key)
{
  char buf[64];
  memset (buf, 0, sizeof(buf));
  sprintf(buf, "AT+CWJAP=\"%s\",\"%s\"", ssid, key);
  return esp8266_send_at(buf, "WIFI GOT IP", 7000);
}

int esp8266_connect(const char *ip, int port)
{
  char buf[64];
  memset (buf, 0, sizeof(buf));
  sprintf(buf, "AT+CIPSTART=\"TCP\",\"%s\",%d", ip, port);
  return esp8266_send_at(buf, "CONNECT", 5000);
}

int esp_open(void)
{
	int rc = 0;

	esp8266_reset();

  esp8266_send_at("AT\r\n", "OK", 2000);

//  esp8266_send_at("AT+CIPSTATUS\r\n", "OK", 2000);

  esp8266_send_at("AT+CWMODE=1\r\n", "OK", 2000);

#if 1
  if ( (rc = esp8266_connect_ap(WIFI_SSID, WIFI_PASSWD)) < 0) {
    printf("connect wifi error!\r\n");
    goto exit;
  }
#endif

exit:
	return rc;
}

int esp_close(void)
{

}

int esp_connect(const char *ip, int port)
{
  int rc = 0;

#if 0
  if ( (rc = esp8266_send_at("AT+CIPMUX=0\r\n", "OK", 2000)) < 0 ) {
    printf("step2\n");
    goto exit;
  }
#endif


  if ( (rc = esp8266_connect(ip, port)) < 0) {
    printf("connect server error!\r\n");
    goto exit;
  }

  if ( (rc = esp8266_send_at("AT+CIPMODE=1\r\n", "OK", 1000)) < 0 ) {
    printf("step3\n");
    goto exit;
  }

  if ( (rc = esp8266_send_at("AT+CIPSEND\r\n", ">", 1000)) < 0 ) {
    printf("step4\n");
    goto exit;
  }

exit:
  return rc;
}


int esp_write(uint8_t *buf, int len)
{
}

int esp_read(uint8_t *buf, int len)
{
}

#endif

