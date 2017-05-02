
#include <ESP8266WiFi.h>
#include "ConfigServer.h"
#include "Ustawienia.h"
#include "OTA.h";
#include "MQTT.h"

bool knownNetwork = false;
int sleep = 60; // sekund

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
		else if (MQTT.check()) // normalna praca
		{
			MQTT.init();
		}
		else // web server do odczytu temperatury lub konfiguracji
		{
			// WebPomiar.init();
		}
	}
	else// tutaj wypada postawic server do konfiguracji...
	{
		ConfigServer.init();
	}
}

void loop() 
{
	if (knownNetwork)
	{
		if (OTA.enabled)
		{
			OTA.loop();
		}
		else if (MQTT.enabled)
		{
			MQTT.loop();
		}
		else // postaw serwer
		{
			// WebPomiar.loop();
		}
	}
	else
	{
		ConfigServer.loop();
	}

	yield();
	delay(100);
}
