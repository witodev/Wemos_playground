
#include "WebData.h"
//#include "HostWiFi.h"
#include "Settings.h"
#include "ConfigServer.h"
#include "OTA.h"
#include "MQTT.h"
#include "MyOLED.h"

void setup() 
{
	Serial.begin(115200);
	MyOLED.init();

	if (Settings.check()) // plik konfiguracji istnieje
	{
		Settings.init(); // odczytaj go
		if (Settings.ConnectToWiFi()) // jesli polaczymy sie do sieci
		{
			String IP(WiFi.localIP().toString());
			MyOLED.print(IP);

			if (OTA.check()) // sprawdz update przez wifi
			{
				OTA.init();
			}
			else if (MQTT.check()) // <-- tutaj normalna praca
			{
				MyOLED.print("MQTT init");
				MQTT.init();
			}
			else // host web server
			{
				MyOLED.print("WebData init");
				WebData.init();
			}
		}
		else // problem z polaczeniem do sieci wifi, tworzymy wlasna
		{
			MyOLED.print("ConfigServer init");
			ConfigServer.init();
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		MyOLED.print("ConfigServer init");
		ConfigServer.init();
	}
}

void loop() 
{
	if (Settings.OK)
	{
		if (OTA.OK) // update przez wifi
		{
			OTA.init();
		}
		else if (MQTT.OK) // <-- tutaj normalna praca
		{
			MQTT.loop();
		}
		else // host web server
		{
			WebData.loop();
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		ConfigServer.init();
	}

	yield();
	delay(10);
}
