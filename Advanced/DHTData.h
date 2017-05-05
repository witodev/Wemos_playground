#pragma once

#include <DHT.h>
#include <ArduinoJson.h>

#define SENSORDATA_DATA_SIZE (JSON_OBJECT_SIZE(3))

class DHTData
{
	DHT* dht;

public:
	DHTData();
	virtual ~DHTData();
	const char* GetJsonData();
};

