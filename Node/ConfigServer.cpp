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
		return NULL;
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
		if (size > 10240) {
			Serial.println("Config file size is too large");
			return NULL;
		}

		//std::unique_ptr<char[]> buf(new char[size]);
		char* buf = new char[size];
		configFile.readBytes(buf, size);
		Serial.print("> File size: ");
		Serial.println(size);

		return buf;
	}
	else
		return NULL;
}

void ConfigServerClass::Handles()
{
	server->on("/", std::bind(&ConfigServerClass::handleRoot, this));
	server->on("/esp.css", std::bind(&ConfigServerClass::handleCSS, this));
	server->on("/config.php", std::bind(&ConfigServerClass::handleConfig, this));
}

void ConfigServerClass::handleRoot()
{
	Serial.println("> Server: handleRoot");
	//const char* index1 = GetPage("indexStart.txt");
	//const char* index2 = GetPage("indexEnd.txt");

	String net = "";
	for (int i = 0; i < netCount; ++i)
	{
		net += "<option value=\""+ String(networks[i].name) +"\">"+ String(networks[i].name) +"</option>";
	}

	//Serial.println("> Server: networks found");

	String page = "";
	//page.concat(index1_html);
	//page.concat(net.c_str());
	page += String(index1_html);
	page += net;
	page += String(index2_html);
		
	server->send(200, "text/html", page);

	page = "";
	//delete[] index1;
	//delete[] index2;
	//Serial.println("> Server: handleRoot end");
}

void ConfigServerClass::handleCSS()
{
	Serial.println("> Server: handleCSS");
	server->send(200, "text/css", esp_css);
}

void ConfigServerClass::handleConfig()
{
	Serial.println("> Server: handleConfig");

	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server->uri();
	message += "\nMethod: ";
	message += (server->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server->args();
	message += "\n";
		
	String json = "{\n";
	uint8_t tmp = 0;
	for (uint8_t i = 0; i<server->args()-1; i++) {
		message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
		json += "\""+ server->argName(i) +"\": \""+ server->arg(i) +"\",\n";
		tmp = i;
	}
	tmp++;
	json += "\"" + server->argName(tmp) + "\": \"" + server->arg(tmp) + "\"\n";
	json += "}";

	server->send(404, "text/plain", message);
		
	//Serial.println(json);
	saveConfig(json.c_str());

	message = "";
}

void ConfigServerClass::saveConfig(const char* msg)
{
	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return;
	}

	// Open config file for writing.
	File configFile = SPIFFS.open("/config.json", "w");
	if (!configFile)
	{
		Serial.println("Failed to open config.json for writing");
		return;
	}

	Serial.print("> Server config: ");
	Serial.println(msg);

	configFile.println(msg);
	configFile.flush();
	configFile.close();
	SPIFFS.end();

	Serial.println("> Server config saved ... Reset ...");

	yield();
	delay(100);
	ESP.restart();
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

