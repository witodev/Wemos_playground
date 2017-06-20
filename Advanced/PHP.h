// PHP.h

#ifndef _PHP_h
#define _PHP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Base.h"
#include <ESP8266WiFi.h>
#include "Settings.h"
#include "DS18B20.h"
#include "MyOLED.h"

class PHPClass: public Base
{
 public:
	 bool check();
	 void init();
	 void loop();
};

extern PHPClass PHP;

#endif

