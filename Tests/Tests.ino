#include <ArduinoJson.h>
#include <FS.h>
#include <OneWire.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SENSORDATA_JSON_SIZE (JSON_OBJECT_SIZE(2))


struct DS18B20
{
	const char* addr;
	float temp;

	bool deserialize(char* json)
	{
		StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.parseObject(json);
		addr = (const char*)root["dev"];
		temp = root["temp"];
		return root.success();
	}

	char* serialize(size_t maxSize)
	{
		char* json = new char[maxSize];
		StaticJsonBuffer<SENSORDATA_JSON_SIZE> jsonBuffer;
		JsonObject& root = jsonBuffer.createObject();
		root["dev"] = addr;
		root["temp"] = temp;
		root.printTo(json, maxSize);
		return json;
	}
};

bool ReadTemp(DS18B20 &result);

bool ReadTemp(DS18B20 &result)
{
	result.addr = "dev1";
	result.temp = 21.32;
}

void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.println("START");

	DS18B20 results;
	ReadTemp(results);

	Serial.print("serialize: ");
	auto json = results.serialize(32);
	Serial.println(json);
	

	Serial.print("deserialize: ");
	DS18B20 data;
	auto ok = data.deserialize(json);
	if (!ok)
		Serial.println("NOT OK");
	auto jsonData = data.serialize(32);

	Serial.println(jsonData);

}
void loop()
{}
