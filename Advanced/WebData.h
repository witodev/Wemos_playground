// WebData.h

#ifndef _WEBDATA_h
#define _WEBDATA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Base.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

class WebDataClass: public Base
{
	ESP8266WebServer* server;

	void handleRoot();
	void drawGraph();
	void handleNotFound();
public:
	bool check();
	void init();
	void loop();
};

extern WebDataClass WebData;

#endif

