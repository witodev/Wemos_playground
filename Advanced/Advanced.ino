
#include "Settings.h"

void setup() 
{
	Serial.begin(115200);

	if (Settings.check())
		Settings.init();

}

void loop() 
{
	if (Settings.OK)
		Settings.loop();

	yield();
	delay(100);
}
