#include <cstdlib>

#include <ESP8266WiFi.h>

#include "Ustawienia.h"
#include "OTA.h";
#include "MQTT.h"

int sleep = 10; // sekund

bool ConnectToKnownNetwork()
{
	WiFi.disconnect();
	
	WiFi.begin(Ustawienia.ssid, Ustawienia.password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
		Serial.print("> Wait for IP: ");
		Serial.println(WiFi.localIP());
	}

	return true;
}

void setup() 
{
	Serial.begin(115200);
	Serial.println();

	Ustawienia.init();

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	
	bool known = ConnectToKnownNetwork();
	if (known)
	{
		if (OTA.check())
		{
			OTA.init();
		}
		else
		{
			MQTT.init();
		}
	}
}

void loop() 
{
	if (OTA.enabled)
	{
		OTA.loop();
	}
	else
	{
		srand(millis());
		float temp = (rand() % 100) / 10.0 + 20.0;
		String t = "{\"dev\":\"dummy\",\"temp\":" + String(temp) + "}";
		MQTT.Send("event", t.c_str());
		ESP.deepSleep(sleep * 1e6);
	}
	yield();
	delay(100);
}
