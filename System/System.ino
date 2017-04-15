
#include "OTA.h"

const char* ssid = "...";
const char* password = "...";
bool otaUpdate = false;

bool ConnectToKnownNetwork()
{
	WiFi.begin(ssid, password);
	//Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
	}

	//Serial.println("");
	//Serial.print("Connected to ");
	//Serial.println(ssid);
	//Serial.print("IP address: ");
	//Serial.println(WiFi.localIP());
	return true;
}

void setup() 
{
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
	else
	{
		yield();
	}
}
