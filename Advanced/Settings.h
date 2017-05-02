// Settings.h

#ifndef _SETTINGS_h
#define _SETTINGS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include <FS.h>
#include "Base.h"

class SettingsClass : public Base
{
private:
	void ShowConfig(const char* title, const char* value);
	char* Convert(const char* source);
public:
	bool check();
	void init();
	void loop();

	const char* ssid;
	const char* password;
	const char* mqtt_user;
	const char* mqtt_pass;
	const char* mqtt_host;
	const char* ota_host;
	const char* ota_page;
};

extern SettingsClass Settings;

#endif

