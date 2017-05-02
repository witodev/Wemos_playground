// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include "Base.h"

class SettingsClass : public Base
{
private:
	void ShowConfig(const char* title, const char* value);
	char* Convert(const char* source);
	StaticJsonBuffer<200> jsonBuffer;
public:
	bool check();
	void init();
	void loop();
	const char* Get(const char* title);

	JsonObject* Data;

	const char* ssid;
	const char* password;
	const char* mqtt_user;
	const char* mqtt_pass;
	const char* mqtt_host;
	const char* ota_host;
	const char* ota_page;

	const char* operator[](const char* title)
	{
		return Get(title);
	}

	bool ConnectToWiFi();
};

extern SettingsClass Settings;

#endif

