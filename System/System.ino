#include <ArduinoJson.h>
#include "FS.h"
#include "OTA.h"

const char* ssid = "";
const char* password = "";

bool sendTemp = true;

bool loadConfig() {
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}
	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		return false;
	}

	ssid = json["ssid"];
	password = json["password"];

	Serial.println();
	Serial.print("> Config ssid: ");
	Serial.println(ssid);
	Serial.print("> Config password: ");
	Serial.println(password);
	return true;
}

bool ConnectToKnownNetwork()
{
	WiFi.disconnect();

	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return false;
	}
	if (!loadConfig())
	{
		Serial.println("Network SSID and password failed");
		return false;
	}
	
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
		Serial.print("> Wait for IP: ");
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
	delay(1000);
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
