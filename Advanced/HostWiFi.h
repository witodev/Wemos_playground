// HostWiFi.h

#ifndef _HOSTWIFI_h
#define _HOSTWIFI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include <cstdio>
#include "Base.h"
#include "Settings.h"

class HostWiFiClass: public Base
{
	ESP8266WebServer* server;

	void handleRoot();
public:
	bool check();
	void init();
	void loop();
};

extern HostWiFiClass HostWiFi;

#endif

