// ConfigServer.h

#ifndef _CONFIGSERVER_h
#define _CONFIGSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <FS.h>
#include <cstdio>

#include "Pages.h"

class ConfigServerClass
{
public:
	struct Network {
		String name;
		int32_t pwr;
	};

private:
	int netCount;
	Network* networks;
	void FindNetworks();
	// base on http://forum.arduino.cc/index.php?topic=38025.0
	void quickSort(Network arr[], int left, int right);

	char* Convert(const char * source, size_t len);
	char* GetPage(const char* title);
	
	void Handles();
	ESP8266WebServer* server;
	void handleRoot();
	void handleCSS();
	void handleConfig();
	void saveConfig(const char* msg);

 protected:


 public:
	void init();
	void loop();

	~ConfigServerClass()
	{
		if (networks != NULL) delete[] networks;
	}
};

extern ConfigServerClass ConfigServer;

#endif

