// MQTT.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Ustawienia.h"

class MQTTClass
{
private:
	WiFiClient espClient;
	PubSubClient client;

protected:
	void callback(char* topic, byte* payload, unsigned int length);

public:
	void init();
	void Send(const char* topic, const char* msg);
};

extern MQTTClass MQTT;

#endif

