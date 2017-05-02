
#include "Settings.h"
#include "ConfigServer.h"
#include "OTA.h"

void setup() 
{
	Serial.begin(115200);

	if (Settings.check())
	{
		Settings.init();
		if (Settings.ConnectToWiFi())
		{
			if (OTA.check())
			{
				OTA.init();
			}
		}
		else // problem z polaczeniem do sieci wifi, tworzymy wlasna
		{
			ConfigServer.init();
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		ConfigServer.init();
	}
}

void loop() 
{
	if (Settings.OK)
	{
		if (OTA.OK)
		{
			OTA.init();
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		ConfigServer.init();
	}

	Serial.println("dummy loop");
	yield();
	delay(100);
}
