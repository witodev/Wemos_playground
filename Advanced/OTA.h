// OTA.h

#ifndef _OTA_h
#define _OTA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Base.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "Settings.h"

class OTAClass : public Base
{
private:
	bool CheckOnRPi();
 protected:


 public:
	void init();
	void loop();
	bool check();
	bool enabled = false;
	bool Host(const char* http = "pizero");
};

extern OTAClass OTA;

#endif

