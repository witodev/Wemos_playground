// MyOLED.h

#ifndef _MYOLED_h
#define _MYOLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>
#include <SSD1306.h> // alias for `#include "SSD1306Wire.h"`

#include <ArduinoJson.h>

#define SENSORDATA_DATA_SIZE (JSON_OBJECT_SIZE(3))

class MyOLEDClass
{
	SSD1306* display;
protected:


public:;
	void init();
	void print(char* json);
	void print(float temp);
	void print(String msg);
};

extern MyOLEDClass MyOLED;

#endif

