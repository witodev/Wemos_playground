// Config.h

#ifndef _CONFIG_h
#define _CONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
	#include <ArduinoJson.h>
	#include <FS.h>
#else
	#include "WProgram.h"
#endif

class ConfigClass
{
private:
	//const char* file = "config.json";
	void ShowConfig(const char* title, const char* value);
	bool LoadConfig();
	char* Convert(const char* source);

 protected:


 public:
	void init();
	
	const char* ssid;
	const char* password;
	const char* mqtt_user;
	const char* mqtt_pass;
	const char* ota_host;
	const char* ota_page;
};

extern ConfigClass Config;

#endif

