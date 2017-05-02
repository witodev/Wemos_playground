// 
// 
// 

#include "Settings.h"

bool SettingsClass::check()
{
	Serial.print("> Settings check ... ");

	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return false;
	}

	if (SPIFFS.exists("/config.json"))
	{
		Serial.println("OK");
		OK = true;
	}
	else
	{
		Serial.println("file does not exists");
		OK = false;
	}

	return OK;
}

void SettingsClass::init()
{
	Serial.println("> Settings init");

	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		OK = false;
		return;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		OK = false;
		return;
	}
	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		OK = false;
		return;
	}

	ssid = Convert(json["ssid"]);
	password = Convert(json["password"]);
	mqtt_user = Convert(json["mqtt_user"]);
	mqtt_pass = Convert(json["mqtt_pass"]);
	mqtt_host = Convert(json["mqtt_host"]);
	ota_host = Convert(json["ota_host"]);
	ota_page = Convert(json["ota_page"]);

	ShowConfig("ssid", ssid);
	ShowConfig("password", password);
	ShowConfig("mqtt_user", mqtt_user);
	ShowConfig("mqtt_pass", mqtt_pass);
	ShowConfig("mqtt_host", mqtt_host);
	ShowConfig("ota_host", ota_host);
	ShowConfig("ota_page", ota_page);

	return;
}

void SettingsClass::loop()
{
	Serial.println("> Settings loop");
}

void SettingsClass::ShowConfig(const char* title, const char* value)
{
	Serial.print("> Config ");
	Serial.print(title);
	Serial.print(": ");
	Serial.print(value);
	Serial.print(" (len: ");
	Serial.print(strlen(value));
	Serial.println(")");
}

char * SettingsClass::Convert(const char * source)
{
	auto len = 100;// strlen(source);
	auto buff = new char[len];
	strcpy(buff, source);
	return buff;
}

SettingsClass Settings;

