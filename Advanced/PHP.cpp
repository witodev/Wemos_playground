// 
// 
// 

#include "PHP.h"

bool PHPClass::check()
{
	Serial.println("> PHP check");
	const char* host = Settings["php_host"];
	const int httpPort = 80;

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	if (!client.connect(host, httpPort)) {
		Serial.println("> PHP host not active");
		return false;
	}
	Serial.println("> PHP host avaliable");
	return true;
}

void PHPClass::init()
{
	Serial.println("> PHP init");
	OK = true;
}

void PHPClass::loop()
{
	const char* host = Settings["php_host"];
	const int httpPort = 80;


	// We now create a URI for the request
	DS18B20 sensor(D3);
	auto json = sensor.GetJsonData();
	if (json == NULL)
		Serial.println("> PHP no more data");

	while (json != NULL)
	{
		// Use WiFiClient class to create TCP connections
		WiFiClient client;
		if (!client.connect(host, httpPort)) {
			Serial.println("> PHP connection failed");
			return;
		}

		DS18B20data data;
		data.deserialize(json);

		float temp = data.temp;
		String dev(data.addr);
		String url = "/" + String(Settings["php_page"]) + String("?dev=") + dev + String("&temp=") + String(temp);

		//Serial.print("> PHP host: ");
		//Serial.println(host);

		String outside("28FF23DB8C160316");
		if (dev.compareTo(outside) == 0)
			MyOLED.print(temp);
		
		Serial.print("> PHP url: ");
		Serial.println(url);

		// This will send the request to the server
		client.print(String("GET ") + url + " HTTP/1.1\r\n" +
			"Host: " + host + "\r\n" +
			"Connection: close\r\n\r\n");

		unsigned long timeout = millis();
		while (client.available() == 0) {
			if (millis() - timeout > 5000) {
				Serial.println("> Client Timeout !");
				client.stop();
				return;
			}
		}

		// Read all the lines of the reply from server and print them to Serial
		String line = "";
		while (client.available()) {
			line = client.readStringUntil('\r');
			//Serial.println(line);
		}
		client.stop();
		json = sensor.GetJsonData();
	}

	int sleep = 60;

	if (sleep == 0)
		return;

	auto timeToSleep = sleep * 1e6 - micros() + 1400 * 1e3;
	Serial.print("Sleep time: ");
	Serial.println(timeToSleep);
	ESP.deepSleep(timeToSleep);
}


PHPClass PHP;

