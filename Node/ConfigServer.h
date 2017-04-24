// ConfigServer.h

#ifndef _CONFIGSERVER_h
#define _CONFIGSERVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

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

 protected:


 public:
	void init();
	void Scan();

	~ConfigServerClass()
	{
		if (networks != NULL) delete[] networks;
	}
};

extern ConfigServerClass ConfigServer;

#endif

