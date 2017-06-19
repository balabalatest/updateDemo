/*
 * update.c
 *
 */

/*** for network layer ********************************************************/

#include "addr.h"
#include <sys/time.h>
#include "socket_port.h"
#include <errno.h>
#include "debug.h"

#include "freeRTOS.h"
#include "task.h"


int socket_read(int sockfd, unsigned char* buffer, int len, int timeout_ms)
{

  int bytes = 0;

  struct timeval interval = {timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  if (interval.tv_sec < 0 || (interval.tv_sec == 0 && interval.tv_usec <= 0))
  {
    interval.tv_sec = 0;
    interval.tv_usec = 100;
  }

  Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&interval, sizeof(struct timeval));

  int rc = Recv(sockfd, buffer, (size_t)(len), 0);
  if (rc == -1)
  {
    if (errno != ENOTCONN && errno != ECONNRESET)
    {
      bytes = -1;
    }
  }
  else
    bytes += rc;

  return bytes;
}


int socket_write(int sockfd, unsigned char* buffer, int len, int timeout_ms)
{
  struct timeval tv;

  tv.tv_sec = 0;  /* 30 Secs Timeout */
  tv.tv_usec = timeout_ms * 1000;  // Not init'ing this can cause strange errors

  Setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));

  int bytes = 0;
  while (bytes < len)
  {
    int rc = Send(sockfd, &buffer[bytes], (size_t)(len - bytes), 0);
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


static void disconnect(int sockfd)
{
  Close(sockfd);
}

static int connect(char* addr, int port)
{
  struct sockaddr_in address;
  struct hostent *hptr;
  int rc = -1;
  int sockfd;

  if ((hptr = Gethostbyname(addr)) == NULL)
    goto exit;

  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr = *((struct in_addr *)hptr->h_addr);

//  address.sin_addr.s_addr = inet_addr(addr);

  if ((sockfd = Socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    goto exit;

  if ((rc = Connect(sockfd, (struct sockaddr*) &address, sizeof(address))) < 0)
  {
    Close(sockfd);
    goto exit;
  }

  rc = sockfd;

exit:
  return rc;
}

/*** flash operation **********************************************************/
#include "image.h"
#include "stm32f10x_flash.h"

#define FLASH_PAGE_SIZE   0x800  /* 2KB */
#define FIRMWARE_B_START  0x08030000UL
#define FIRMWARE_B_SIZE   0x10000UL

image_header_t header;

static int isopen = 0;

int flash_open(void)
{
  image_header_t *hdr = &header;
  uint32_t size = ntohl(hdr->ih_size);

  uint32_t FlashAddress = FIRMWARE_B_START - FLASH_PAGE_SIZE;
  uint32_t LastAddress  = FIRMWARE_B_START + size;

  if (isopen)
    return 0;
  isopen = 1;

  FLASH_Unlock();

  while (FlashAddress <= (uint32_t) LastAddress) {
    if (FLASH_ErasePage(FlashAddress) == FLASH_COMPLETE) {
      FlashAddress += FLASH_PAGE_SIZE;
    } else {
      printf("Flash erase error\n");
      FLASH_Lock();
      return -1;
    }
  }
  return 0;
}

int flash_close(void)
{
  if (!isopen)
    return 0;
  isopen = 0;

  FLASH_Lock();
  return 0;
}


int flash_write(uint32_t offset, uint8_t *pdata, uint16_t DataLength)
{
  int i = 0;

  uint32_t FlashAddress = FIRMWARE_B_START - sizeof(image_header_t) + offset;
  uint32_t LastAddress  = FIRMWARE_B_START + FIRMWARE_B_SIZE;

  assert( (FlashAddress % 4) == 0 );

  uint16_t count = DataLength / 4;

  if (DataLength % 4)
    count++;

  for (int i = 0; i < count; i++) {

    if (FlashAddress >= LastAddress)
      return -1;

    if ( FLASH_ProgramWord(FlashAddress,  *((uint32_t *)pdata)) != FLASH_COMPLETE )
      return -1;

    pdata        += 4;
    FlashAddress += 4;
  }

  return 0;
}

/*** http download ************************************************************/



#define SL_ATTRIBUTE_ALIGN(X) __attribute__ ((aligned(X)))
#define STATIC_UBUF(x, y) static uint8_t x[((y)+3)&~3] SL_ATTRIBUTE_ALIGN(4)

STATIC_UBUF( buf, 2048 );


static void address_cut_tail(char *buf)
{
	char *p;

	for (p = buf; (*p != '/') && (*p != 0); p++);
	*p = 0;
}

#define LOAD_SIZE  1024

int http_download(const char *fulladdr)
{
  int ret = -1;

  int per = LOAD_SIZE;
  int offset = 0;
  int last = 0;

  int len;
  uint8_t *ptr;

  char tmp[32];
  assert( strlen(fulladdr) < sizeof(tmp) );
  strcpy(tmp, fulladdr);
  address_cut_tail(tmp);

  printf("Start http update ...\n");

//  printf("Connecting server...\n");
//  printf("Connect success to %s : %d\n", tmp, 80);

  int fd = connect(tmp, 80);
  if (fd == -1) {
    printf("ERROR: Failed to connect http server\n");
    return -1;
  }

  int retry = 10;

#if 1
extern void led_task(void*);
static TaskHandle_t ledHandle;
xTaskCreate(led_task, "Task-led4", 1024, NULL, configMAX_PRIORITIES-4, &ledHandle);
#endif

  do {

    snprintf(buf, sizeof(buf), "GET http://%s HTTP/1.1\r\nRange: bytes=%d-%d\r\nHost: %s\r\n\r\n", fulladdr, offset, offset+per-1, tmp);
//    snprintf(buf, sizeof(buf), "GET http://%s HTTP/1.1\r\nRange: bytes=%d-%d\r\nConnection: close\r\nHost: %s\r\n\r\n", opts.from, offset, offset+per-1, tmp);

    ret = socket_write(fd, buf, strlen(buf), 10 * 1000);
    if (ret < 0) {
      printf("Error CONNECT in write, offset = %d\n", offset);
      if (retry--)
        continue;
      goto quit;
    }
    if (ret != strlen(buf)) {
      printf("Error: Socket Send %dBytes\n", ret);
      continue;
    }

    memset(buf, 0, sizeof(buf));
    ret = socket_read(fd, buf, sizeof(buf), 10 * 1000);
    if (ret < 0) {
      printf("Error CONNECT in read, offset = %d\n", offset);
      if (retry--)
        continue;
      goto quit;
    }

#if 1
sscanf(buf, "HTTP/1.1 %d ", &len);
if (len != 206) {
  printf("HTTP RC = %d\n", len);
}
#endif

    ptr = strstr(buf, "Content-Length: ");

    if (ptr == NULL) {
      printf("Packet error\n");
      continue;
    }

    sscanf(ptr, "Content-Length: %d\r\n", &len);

    if (len > per) {
      printf("Packet error\n");
      continue;
    }

    ptr = strstr(ptr, "\r\n\r\n");
    ptr += 4;

    if (offset == 0) {
      image_header_t *hdr = &header;

      memset(hdr, 0, sizeof(image_header_t));
      memcpy(hdr, ptr, sizeof(image_header_t));

      if (ntohl(hdr->ih_magic) != IH_MAGIC) {
        printf("uImage has bad header:MAGIC\n");
        goto quit;
      }

      if (1) { /* check for header crc */
        uint32_t checksum = ntohl(hdr->ih_hcrc);

        hdr->ih_hcrc = htonl(0);  /* clear for re-calculation */

        if (crc32 (0, (char*)hdr, sizeof(image_header_t)) != checksum) {
          printf("uImage has bad header:CRC\n");
          goto quit;
        }
      }

      if (0) { /* check for version */

        uint32_t vtor = (volatile uint32_t)SCB->VTOR;
        if (vtor & NVIC_VectTab_FLASH) {

          image_header_t *pheader = (image_header_t *) (vtor - sizeof(image_header_t));
          time_t time_early = (time_t)ntohl(pheader->ih_time);
          time_t time_now   = (time_t)ntohl(hdr->ih_time);

          if (time_now <= time_early) {
            printf("uImage is an early version");
            goto quit;
          }
        }
      }

      last = ntohl(header.ih_size) + sizeof(image_header_t);

//      print_header(&header);

      if (flash_open() < 0) {
        goto quit;
      }

      printf("\nDownload ...\n");

    }

    flash_write(offset, ptr, len);

    offset += len;
    last   -= len;

/* TODO print progress bar: 99% */
#if 1
static int prev = 0;
  int percent = 10 * ((offset * 10) / (ntohl(header.ih_size) + sizeof(image_header_t)));
  if (percent != prev) {
    prev = percent;
    printf("%d %%\n", percent);
  }
#endif



    if (last < per)
      per = last;

    if (last == 0) {

      image_header_t *hdr = &header;
      uint32_t size = ntohl(hdr->ih_size);
      uint32_t checksum = ntohl(hdr->ih_dcrc);

      char *data = (char *) FIRMWARE_B_START;

      printf("\nDownloaded Succeed, Got %u : %u \n\n", offset , (size + sizeof(image_header_t)));

      if (crc32 (0, data, size) != checksum) {
        printf("uImage data CRC error\n");
        goto quit;
      }

      break;
    }

  } while (last > 0);

  ret = offset;

quit:

#if 1
vTaskDelete( ledHandle );
#endif

  flash_close();
//  printf("Quit http download\n");
  disconnect(fd);

  return ret;
}


