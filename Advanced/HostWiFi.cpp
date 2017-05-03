// 
// 
// 

#include "HostWiFi.h"

void HostWiFiClass::handleRoot()
{
	server->send(200, "text/html", "<h1>You are connected</h1>");
}

bool HostWiFiClass::check()
{
	return false;
}

void HostWiFiClass::init()
{
	server = new ESP8266WebServer(80);
	Serial.print("> HostWifi: Configuring access point...");
	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(Settings["ssid"], Settings["password"]);

	IPAddress myIP = WiFi.softAPIP();
	Serial.print("> HostWIfi: AP IP address: ");
	Serial.println(myIP);

	server->on("/", std::bind(&HostWiFiClass::handleRoot, this));
	server->begin();
	Serial.println("> HostWifi: HTTP server started");
}

void HostWiFiClass::loop()
{
	server->handleClient();
}


HostWiFiClass HostWiFi;

