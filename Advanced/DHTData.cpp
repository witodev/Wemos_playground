#include "DHTData.h"



DHTData::DHTData()
{
	dht = new DHT(D4, DHT22);
	dht->begin();
}


DHTData::~DHTData()
{
}

const char * DHTData::GetJsonData()
{
	float h = dht->readHumidity();
	// Read temperature as Celsius (the default)
	float t = dht->readTemperature();

	// Check if any reads failed and exit early (to try again).
	if (isnan(h) || isnan(t)) {
		Serial.println("> DHT: Failed to read from DHT sensor!");
		return nullptr;
	}
	// Compute heat index in Celsius (isFahreheit = false)
	float hic = dht->computeHeatIndex(t, h, false);
	
	size_t maxSize = 50;
	char* json = new char[maxSize];
	StaticJsonBuffer<SENSORDATA_DATA_SIZE> jsonBuffer;
	JsonObject& root = jsonBuffer.createObject();
	root["dev"] = "hum";
	root["temp"] = hic;
	root["hum"] = h;
	root.printTo(json, maxSize);
	return json;
}
