
#include "OTA.h"

//const char* ssid = "...";
//const char* password = "...";

bool sendTemp = true;

bool ConnectToKnownNetwork()
{
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
		Serial.print("> IP: ");
		Serial.println(WiFi.localIP());
	}

	Serial.println("");
	Serial.print("> Connected to ");
	Serial.println(ssid);
	Serial.print("> IP address: ");
	Serial.println(WiFi.localIP());

	return true;
}

void setup() 
{
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	bool known = ConnectToKnownNetwork();
	if (known) 
	{
		if (OTA.check())
			OTA.init();
	}
	//else
	//{
	//	HostServerConfig();
	//}
}

void loop() 
{
	if (OTA.enabled)
	{
		OTA.loop();
	}
	else if (sendTemp)
	{
		Serial.println("Tutaj bedziemy pobierac temperature i wysylac ja przez MQTT do Node-RED");
		Serial.println("No a pozniej pojdziemy spac...");
		//ESP.deepSleep();
		delay(1000);
	}

	yield();
}
