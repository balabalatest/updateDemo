/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "freeRTOS-esp8266.h"


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


#include "MQTTClient.h"


//#define CONFIG_SERV_IP        "183.230.40.39"
#define CONFIG_SERV_IP        "jjfaedp.hedevice.com"
#define CONFIG_SERV_PORT       6002
#define CONFIG_WIFI_SSID      "CMCC-IOT"
#define CONFIG_WIFI_PASSWORD  "IOT@Chinamobile"


void messageArrived(MessageData* data)
{
	printf("Message arrived on topic %.*s: %.*s\n", data->topicName->lenstring.len, data->topicName->lenstring.data,
		data->message->payloadlen, data->message->payload);
}

void prvMQTTEchoTask(void *pvParameters)
{
	/* connect to m2m.eclipse.org, subscribe to a topic, send and receive messages regularly every 1 sec */
	MQTTClient client;
	Network network;
	unsigned char sendbuf[80], readbuf[80];
	int rc = 0,	count = 0;
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;

	pvParameters = 0;

	NetworkInit(&network);

  printf("Connecting to %s %d\n", CONFIG_SERV_IP, CONFIG_SERV_PORT);

  if ((rc = NetworkConnect(&network, CONFIG_SERV_IP, CONFIG_SERV_PORT)) != 0)
    printf("Return code from network connect is %d\n", rc);


	MQTTClientInit(&client, &network, 3*1000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

  int qos = 0;
	char *topic = "topic123";
  connectData.clientID.cstring = "6296722";
  connectData.username.cstring = "86338";
  connectData.password.cstring = "rtos";

	connectData.keepAliveInterval = 120;
	connectData.cleansession = 1;
	connectData.MQTTVersion = 4;

	if ((rc = MQTTConnect(&client, &connectData)) != 0) {
		printf("Return code from MQTT connect is %d\n", rc);
		while (1);
	}
	else
		printf("MQTT Connected\n");

	if ((rc = MQTTSubscribe(&client, topic, qos, messageArrived)) != 0) {
		printf("Return code from MQTT subscribe is %d\n", rc);
//		while (1);
	}
  printf("MQTT Subscribed\n");

	while (++count)
	{
		MQTTMessage message;
		char payload[30];

		message.qos = 0;
		message.retained = 0;
		message.payload = payload;
		sprintf(payload, "message number %d", count);
		message.payloadlen = strlen(payload);

		if ((rc = MQTTPublish(&client, topic, &message)) != 0)
			printf("Return code from MQTT publish is %d\n", rc);

#if !defined(MQTT_TASK)
		if ((rc = MQTTYield(&client, 3000)) != 0)
			printf("Return code from yield is %d\n", rc);
#endif
	}

	/* do not return */
}


void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	BaseType_t x = 0L;

	xTaskCreate(prvMQTTEchoTask,	/* The function that implements the task. */
			"MQTTEcho0",			/* Just a text name for the task to aid debugging. */
			usTaskStackSize,	/* The stack size is defined in FreeRTOSIPConfig.h. */
			(void *)x,		/* The task parameter, not used in this case. */
			uxTaskPriority,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
			NULL);				/* The task handle is not used. */
}
/*-----------------------------------------------------------*/


