
#include "PHP.h"
#include "WebData.h"
//#include "HostWiFi.h"
#include "Settings.h"
#include "ConfigServer.h"
#include "OTA.h"
#include "MQTT.h"
#include "MyOLED.h"

auto work = PHP;

void setup() 
{
	Serial.begin(115200);
	MyOLED.init();

	if (Settings.check()) // plik konfiguracji istnieje
	{
		Settings.init(); // odczytaj go
		MyOLED.print(String("Waiting for IP..."));
		if (Settings.ConnectToWiFi()) // jesli polaczymy sie do sieci
		{
			String IP(WiFi.localIP().toString());
			MyOLED.print(IP);

			if (OTA.check()) // sprawdz update przez wifi
			{
				MyOLED.print(String("OTA init"));
				OTA.init();
			}
			else if (work.check()) // <-- tutaj normalna praca
			{
				MyOLED.print(String("WORK init"));
				work.init();
			}
			else // host web server
			{
				MyOLED.print(String("WebData init"));
				WebData.init();
			}
		}
		else // problem z polaczeniem do sieci wifi, tworzymy wlasna
		{
			MyOLED.print(String("ConfigServer init"));
			ConfigServer.init();
		}
	}
	else // jesli ustawienia zawioda to postaw server konfiguracyjny
	{
		MyOLED.print(String("ConfigServer init"));
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
		else if (work.OK) // <-- tutaj normalna praca
		{
			work.loop();
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
