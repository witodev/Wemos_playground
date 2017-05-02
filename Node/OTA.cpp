// 
// 
// 

#include "OTA.h"

bool OTAClass::CheckOnRPi()
{
	const char* host = Ustawienia.ota_host;
	const int httpPort = 80;

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	if (!client.connect(host, httpPort)) {
		Serial.println("> OTA connection failed");
		return false;
	}

	// We now create a URI for the request
	String url = "/" + String(Ustawienia.ota_page) + WiFi.hostname();
	
	//Serial.print("Host: ");
	//Serial.println(host);

	//Serial.print("Requesting URL: ");
	//Serial.println(url);

	// This will send the request to the server
	client.print(String("GET ") + url + " HTTP/1.1\r\n" +
		"Host: " + host + "\r\n" +
		"Connection: close\r\n\r\n");

	unsigned long timeout = millis();
	while (client.available() == 0) {
		if (millis() - timeout > 5000) {
			Serial.println("> Client Timeout !");
			client.stop();
			return false;
		}
	}

	// Read all the lines of the reply from server and print them to Serial
	String line = "";
	while (client.available()) {
		line = client.readStringUntil('\r');
		//Serial.println(line);
	}

	line.trim();

	//Serial.print("Line: ");

	//Serial.print("At: ");
	//Serial.println(line.charAt(0));

	if (line.length() == 1)
	{
		if (line.charAt(0) == '1')
			return true;
		else
			return false;
	}

	return false;
}

void OTAClass::init()
{			
	// Port defaults to 8266
	 ArduinoOTA.setPort(8266);

	// Hostname defaults to esp8266-[ChipID]
	// ArduinoOTA.setHostname("myesp8266");

	// No authentication by default
	// ArduinoOTA.setPassword((const char *)"123");

	ArduinoOTA.onStart([]() {
		Serial.println("Start");
	});
	ArduinoOTA.onEnd([]() {
		Serial.println("\nEnd");
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\n\r", (progress / (total / 100)));
	});
	ArduinoOTA.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) Serial.println("End Failed");
	});
	ArduinoOTA.begin();
	Serial.println("> OTA Configured");
}

void OTAClass::loop()
{
	ArduinoOTA.handle();
}

bool OTAClass::check()
{
	// zakladamy ze juz jestesmy polaczeni do sieci
	// teraz do sprawdzimy czy trzeba robic update, zmienna net
	
	String dev = WiFi.hostname();
	Serial.print("> OTA device: ");
	Serial.println(dev);
	
	bool net = CheckOnRPi(); // tutaj trzeba polaczyc sie z raspberry pi i sprawdzic czy bedziemy updateowac to urzadzenie

	Serial.print("> OTA ");
	Serial.println((net ? "Enabled" : "Disabled"));
	digitalWrite(LED_BUILTIN, (net ? LOW : HIGH));
	delay(100);
	enabled = net;

	//if (net)
	//{
	//	Serial.println("> OTA Enabled");
	//	digitalWrite(LED_BUILTIN, LOW);
	//	delay(100);
	//	enabled = true;
	//}
	//else
	//{
	//	Serial.println("> OTA Disabled");
	//	digitalWrite(LED_BUILTIN, HIGH);
	//	delay(100);
	//	enabled = false;
	//}

	return net;
}

bool OTAClass::Host(const char * http)
{
	return false;
}


OTAClass OTA;

