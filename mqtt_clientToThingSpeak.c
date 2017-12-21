// Based on http://www.eclipse.org/paho/clients/c/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#include "APIKEY.h"

#define ADDRESS     "mqtt.thingspeak.com:1883"
#define CLIENTID    "388132"
#define TOPIC       "channels/388132/publish/fields/field1/"
#define QOS         0
#define TIMEOUT     10000L

int main(int argc, char* argv[])
{
int n = strlen(TOPIC) + strlen(myAPIKEY);
char* TOPICWITHKEY;
TOPICWITHKEY = (char *) malloc(n+1);

strcpy(TOPICWITHKEY,TOPIC);
strcat(TOPICWITHKEY,myAPIKEY);

printf(TOPICWITHKEY);
printf("\n");

char* PAYLOAD;
PAYLOAD = (char *) malloc(strlen(argv[1])+1);
strcpy(PAYLOAD,argv[1]);
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPICWITHKEY, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPICWITHKEY, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
