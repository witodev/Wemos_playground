// Ustawienia.h

#ifndef _USTAWIENIA_h
#define _USTAWIENIA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

	#include <ArduinoJson.h>
	#include <FS.h>

class UstawieniaClass
{
 protected:
	 bool LoadConfig();
	 void ShowConfig(const char* title, const char* value);
	 char* Convert(const char* source);

 public:
	void init();

	const char* ssid;
	const char* password;
	const char* mqtt_user;
	const char* mqtt_pass;
	const char* mqtt_host;
	const char* ota_host;
	const char* ota_page;

};

extern UstawieniaClass Ustawienia;

#endif

