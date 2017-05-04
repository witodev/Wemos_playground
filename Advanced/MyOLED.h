// MyOLED.h

#ifndef _MYOLED_h
#define _MYOLED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN

class MyOLEDClass
{
	Adafruit_SSD1306* display;
protected:


public:
	void init();
	void print(const char* msg);
	void print(float temp);
};

extern MyOLEDClass MyOLED;

#endif

