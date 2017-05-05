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


#define OLED_RESET LED_BUILTIN

class MyOLEDClass
{
	SSD1306* display;
protected:


public:;
	void init();
	void print(const char* msg);
	void print(float temp);
};

extern MyOLEDClass MyOLED;

#endif

