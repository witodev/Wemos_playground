#include <ESP8266WiFi.h>

#include "Ustawienia.h"
#include "OTA.h";

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

	Ustawienia.init();
	
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
	
	bool known = ConnectToKnownNetwork();
	if (known)
	{
		if (OTA.check())
			OTA.init();
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

	}
}
