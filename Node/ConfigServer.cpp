// 
// 
// 

#include "ConfigServer.h"

void ConfigServerClass::FindNetworks()
{
	if (networks != NULL)
		delete[] networks;

	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	// WiFi.scanNetworks will return the number of networks found
	netCount = WiFi.scanNetworks();
	//Serial.println("scan done");

	networks = new Network[netCount];

	if (netCount == 0)
		Serial.println("> Scan: no network found");
	else
	{
		//Serial.print(n);
		//Serial.println(" networks found");
		for (int i = 0; i < netCount; ++i)
		{
			networks[i] = { WiFi.SSID(i), WiFi.RSSI(i) };
		}

		quickSort(networks, 0, netCount);

		for (int i = 0; i < netCount; ++i)
		{
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(networks[i].name);
			Serial.print(" (");
			Serial.print(networks[i].pwr);
			Serial.println(")");
		}
	}
	//Serial.println("");
	//// Wait a bit before scanning again
	//delete[] networks;
	//delay(10000);
}

void ConfigServerClass::quickSort(Network arr[], int left, int right)
{
	int i = left, j = right;
	Network tmp;
	Network pivot = arr[(left + right) / 2];

	/* partition */
	while (i <= j) {
		while (arr[i].pwr < pivot.pwr)
			i++;
		while (arr[j].pwr > pivot.pwr)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	};

	/* recursion */
	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

char * ConfigServerClass::Convert(const char * source, size_t len = 100)
{
	auto buff = new char[len];
	strcpy(buff, source);
	return buff;
}

char * ConfigServerClass::GetPage(const char * title)
{
	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return;
	}

	String filename = "/" + String(title);
	Serial.print("> GetPage: ");
	Serial.println(filename);

	if (SPIFFS.exists(filename))
	{
		File configFile = SPIFFS.open(filename, "r");
		if (!configFile) {
			Serial.println("Failed to open config file");
			return NULL;
		}

		size_t size = configFile.size();
		if (size > 2048) {
			Serial.println("Config file size is too large");
			return NULL;
		}
		std::unique_ptr<char[]> buf(new char[size]);
		configFile.readBytes(buf.get(), size);
		
		return Convert(buf.get(), size);
	}
	else
		return NULL;
}

void ConfigServerClass::Handles()
{
	server->on("/", std::bind(&ConfigServerClass::handleRoot, this));
	server->on("/esp.css", std::bind(&ConfigServerClass::handleCSS, this));
}

void ConfigServerClass::handleRoot()
{
	server->send(200, "text/html", index_html);
}

void ConfigServerClass::handleCSS()
{
	server->send(200, "text/css", esp_css);
}

void ConfigServerClass::init()
{
	networks = NULL;
	Serial.println("> Server init");
	
	FindNetworks();

	WiFi.disconnect();

	WiFi.mode(WIFI_AP);
	WiFi.softAP("ConfigServer", "12345678");
	
	server = new ESP8266WebServer(80);
	IPAddress myIP = WiFi.softAPIP();
	Serial.print("> Server IP: ");
	Serial.println(myIP);
	Handles();
	server->begin();

}

void ConfigServerClass::loop()
{
	server->handleClient();
}


ConfigServerClass ConfigServer;

