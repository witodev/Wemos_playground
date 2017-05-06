// 
// 
// 

#include "WebData.h"
#include "DS18B20.h"
#include "DHTData.h"
#include "MyOLED.h"

void WebDataClass::handleRoot()
{
	DS18B20 sensor(D3);
	DHTData hum;

	auto tempJson = sensor.GetJsonData();
	auto humJson = hum.GetJsonData();
	
	char temp[400];
	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	snprintf(temp, 400,

		"<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Hackerspace</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from HackerSpace!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
	<p>Temp: %s</p>\
	<p>Hum: %s</p>\
  </body>\
</html>",

hr, min % 60, sec % 60, tempJson, humJson
);
	server->send(200, "text/html", temp);

	MyOLED.print((char*)humJson);
}

void WebDataClass::drawGraph()
{
	String out = "";
	char temp[100];
	out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
	out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
	out += "<g stroke=\"black\">\n";
	int y = rand() % 130;
	for (int x = 10; x < 390; x += 10) {
		int y2 = rand() % 130;
		sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
		out += temp;
		y = y2;
	}
	out += "</g>\n</svg>\n";

	server->send(200, "image/svg+xml", out);
}

void WebDataClass::handleNotFound()
{
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server->uri();
	message += "\nMethod: ";
	message += (server->method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server->args();
	message += "\n";

	for (uint8_t i = 0; i < server->args(); i++) {
		message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
	}

	server->send(404, "text/plain", message);
}

bool WebDataClass::check()
{
	return false;
}

void WebDataClass::init()
{
	server = new ESP8266WebServer(80);
	
	if (MDNS.begin("esp8266")) {
		Serial.println("> Web: MDNS responder started");
	}

	server->on("/", std::bind(&WebDataClass::handleRoot, this));
	server->on("/test.svg", std::bind(&WebDataClass::drawGraph, this));
	server->onNotFound(std::bind(&WebDataClass::handleNotFound, this));
	server->begin();
	Serial.println("> Web: HTTP server started");
}

void WebDataClass::loop()
{
	server->handleClient();
}


WebDataClass WebData;

