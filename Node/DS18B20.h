// DS18B20.h

#ifndef _DS18B20_h
#define _DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>

class DS18B20
{
private:
	OneWire* ds;
protected:


public:
	DS18B20(uint8_t pin);
	~DS18B20();

	float ReadTempCelcius();
};

#endif

