#include "ConfigServer.h"
#include <ESP8266WiFi.h>

#include "Ustawienia.h"
#include "OTA.h";
#include "MQTT.h"
#include "DS18B20.h"

bool knownNetwork = false;
int sleep = 60; // sekund
DS18B20 sensor(D7);

bool ConnectToKnownNetwork()
{
	if (!Ustawienia.init())
		return false;

	WiFi.disconnect();

	WiFi.mode(WIFI_STA);
	WiFi.begin(Ustawienia.ssid, Ustawienia.password);
	Serial.println("");

	// Wait for connection
	ulong wait = 1e4;
	ulong cur = millis();
	while (WiFi.status() != WL_CONNECTED && (millis()<(cur+wait))) {
		delay(500);
		//Serial.print(".");
		Serial.print("> Wait for IP: ");
		Serial.println(WiFi.localIP());
	}

	if (millis() >= (cur + wait))
		return false;

	return true;
}

void setup() 
{
	Serial.begin(115200);
	Serial.println();
		
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	
	knownNetwork = ConnectToKnownNetwork();
	if (knownNetwork)
	{
		if (OTA.check())
		{
			OTA.init();
		}
		else // normalna praca
		{
			MQTT.init();
		}
	}
	else// tutaj wypada postawic server do konfiguracji...
	{
		ConfigServer.init();
	}
}

void loop() 
{
	if (OTA.enabled)
	{
		OTA.loop();
	}
	else if (knownNetwork) // normalna praca
	{
		auto temp = sensor.GetJsonData();
		while (temp != NULL)
		{
			MQTT.Send("event", temp);
			temp = sensor.GetJsonData();
		}

		auto timeToSleep = sleep * 1e6 - micros() + 1400 * 1e3;
		Serial.print("Sleep time: ");
		Serial.println(timeToSleep);
		ESP.deepSleep(timeToSleep);
	}
	else// czekaj na klienta do konfiguracji
	{		
		ConfigServer.loop();
	}
	yield();
	delay(100);
}
