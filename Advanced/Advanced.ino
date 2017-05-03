
#include "Settings.h"
#include "ConfigServer.h"
#include "OTA.h"
#include "MQTT.h"

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
			else if (MQTT.check()) // <-- tutaj normalna praca
			{
				MQTT.init();
			}
			else // host web server
			{
				Serial.println("web server init");
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
		else if (MQTT.OK) // <-- tutaj normalna praca
		{
			MQTT.loop();
		}
		else // host web server
		{
			Serial.println("web server loop");
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		ConfigServer.init();
	}

	Serial.println("yield loop");
	yield();
	delay(100);
}
