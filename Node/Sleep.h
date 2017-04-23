// Sleep.h

#ifndef _SLEEP_h
#define _SLEEP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SleepClass
{
private:
	ulong time;
 protected:


 public:
	 SleepClass() :time(0) {}

	 void Sleep(ulong t)
	 {
		 time += t;
		 delay(t);
	 }

	 ulong Count()
	 {
		 return time;
	 }
};

extern SleepClass Sleep;

#endif

