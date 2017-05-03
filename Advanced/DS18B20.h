// DS18B20.h

#ifndef _DS18B20_h
#define _DS18B20_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <OneWire.h>
#include <ArduinoJson.h>

#define SENSORDATA_JSON_SIZE (JSON_OBJECT_SIZE(2))

struct DS18B20data
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

class DS18B20
{
private:
	OneWire* ds;

	char* Convert(const char* source)
	{
		auto len = 100;// strlen(source);
		auto buff = new char[len];
		strcpy(buff, source);
		return buff;
	}

protected:


public:
	DS18B20(uint8_t pin);
	~DS18B20();

	float ReadTempCelcius();
	char* GetJsonData();
	DS18B20data GetData();
};


#endif

