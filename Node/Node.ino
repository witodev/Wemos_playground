#include <ESP8266WiFi.h>

#include "Ustawienia.h"
#include "OTA.h";
#include "MQTT.h"
#include "DS18B20.h"

int sleep = 60; // sekund
DS18B20 sensor(D7);

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
		float temp = sensor.ReadTempCelcius();
		while (temp != NULL)
		{
			String t = "{\"dev\":\"dummy\",\"temp\":" + String(temp) + "}";
			MQTT.Send("event", t.c_str());
			temp = sensor.ReadTempCelcius();
		}

		auto timeToSleep = sleep * 1e6 - micros() + 270 * 1e3;
		Serial.print("Sleep time: ");
		Serial.println(timeToSleep);
		ESP.deepSleep(timeToSleep);
	}
	yield();
	delay(100);
}
