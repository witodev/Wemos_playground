// Ustawienia.h

#ifndef _USTAWIENIA_h
#define _USTAWIENIA_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class UstawieniaClass
{
 protected:


 public:
	void init();
};

extern UstawieniaClass Ustawienia;

#endif

