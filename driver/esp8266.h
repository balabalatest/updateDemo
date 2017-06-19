#ifndef _ESP8266_H_
#define _ESP8266_H_

#include <stdio.h>

int esp_open(void);
int esp_close(void);
int esp_connect(const char *ip, int port);
int esp_write(uint8_t *buf, int len, int timeout);
int esp_read(uint8_t *buf, int len, int timeout);

#endif
