// MQTT.h

#ifndef _MQTT_h
#define _MQTT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Base.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Settings.h"
#include "DS18B20.h"

#include "MyOLED.h"

class MQTTClass : public Base
{
private:
	WiFiClient espClient;
	PubSubClient client;

protected:
	void callback(char* topic, byte* payload, unsigned int length);

public:
	const int sleep = 0;
	bool check();
	void init();
	void loop();
	void Send(const char* topic, const char* msg);
};

extern MQTTClass MQTT;

#endif

