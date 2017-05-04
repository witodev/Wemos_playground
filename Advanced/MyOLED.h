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

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

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

