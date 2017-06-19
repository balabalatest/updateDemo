#ifndef __SOCKET_PORT__
#define __SOCKET_PORT__


#define _ESP8266


#ifdef _ESP8266
#include "freeRTOS-esp8266.h"

#define Socket         lesp_socket
#define Setsockopt     lesp_setsockopt
#define Connect        lesp_connect
#define Close          lesp_closesocket
#define Recv           lesp_recv
#define Send           lesp_send
#define Select
#define Accept         lesp_accept
#define Bind           lesp_bind
#define Listen         lesp_listen
#define Gethostbyname  lesp_gethostbyname

#else

#define Socket         socket
#define Setsockopt     setsockopt
#define Connect        connect
#define Close          close
#define Recv           recv
#define Send           send
#define Select         select
#define Accept         accept
#define Bind           bind
#define Listen         listen
#define Gethostbyname  gethostbyname

#endif


#endif

