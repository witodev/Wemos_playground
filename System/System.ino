#include <ArduinoJson.h>
#include <FS.h>
#include <OneWire.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "OTA.h"

OneWire  ds(D7);  // on pin 10 (a 4.7K resistor is necessary)

const char* ssid = "";
const char* password = "";
const char* mqtt_server = "pizero";
char NR_user[8];
char NR_pass[8];

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

bool sendTemp = true;
ulong start = millis();
ulong sleep = 55;

struct DS18B20
{
	String addr;
	float temp;
};

bool ReadTemp(DS18B20 &result);
bool SendToRPi(DS18B20 &result);
void reconnect(DS18B20 &result);

void callback(char* topic, byte* payload, unsigned int length) {
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
	}
	Serial.println();

	// Switch on the LED if an 1 was received as first character
	if ((char)payload[0] == '1') {
		digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
										  // but actually the LED is on; this is because
										  // it is acive low on the ESP-01)
	}
	else {
		digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
	}

}

void reconnect(DS18B20 &result) {
	// Loop until we're reconnected
	Serial.print("> MQTT user: ");
	Serial.println(NR_user);
	Serial.print("> MQTT pass: ");
	Serial.println(NR_pass);

	String msg = "" + result.addr + " " + result.temp;

	while (!client.connected()) {
		Serial.print("Attempting MQTT connection...");
		// Attempt to connect
		if (client.connect("esp", NR_user, NR_pass)) {
			Serial.println("connected");
			Serial.println(msg);
			// Once connected, publish an announcement...
			client.publish("event", msg.c_str());
			// ... and resubscribe
			client.subscribe("event");
		}
		else {
			Serial.print("failed, rc=");
			Serial.print(client.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}
void setupMQTT()
{
	client.setServer(mqtt_server, 1883);
	client.setCallback(callback);
}

bool loadConfig() {
	File configFile = SPIFFS.open("/config.json", "r");
	if (!configFile) {
		Serial.println("Failed to open config file");
		return false;
	}

	size_t size = configFile.size();
	if (size > 1024) {
		Serial.println("Config file size is too large");
		return false;
	}
	std::unique_ptr<char[]> buf(new char[size]);
	configFile.readBytes(buf.get(), size);

	StaticJsonBuffer<200> jsonBuffer;
	JsonObject& json = jsonBuffer.parseObject(buf.get());

	if (!json.success()) {
		Serial.println("Failed to parse config file");
		return false;
	}

	ssid = json["ssid"];
	password = json["password"];

	const char* tmp1 = json["NR_user"];
	strcpy(NR_user, (const char*)tmp1);
	Serial.print("> NR_user: ");
	Serial.print(NR_user);
	Serial.println(".");

	const char* tmp2 = json["NR_pass"];
	strcpy(NR_pass, (const char*)tmp2);
	Serial.print("> NR_pass: ");
	Serial.print(NR_pass);
	Serial.println(".");

	Serial.println();
	Serial.print("> Config ssid: ");
	Serial.println(ssid);
	Serial.print("> Config password: ");
	Serial.println(password);
	return true;
}

bool ConnectToKnownNetwork()
{
	WiFi.disconnect();

	if (!SPIFFS.begin()) {
		Serial.println("Failed to mount file system");
		return false;
	}
	if (!loadConfig())
	{
		Serial.println("Network SSID and password failed");
		return false;
	}
	
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
		Serial.print("> Wait for IP: ");
		Serial.println(WiFi.localIP());
	}

	Serial.println("");
	Serial.print("> Connected to ");
	Serial.println(ssid);
	Serial.print("> IP address: ");
	Serial.println(WiFi.localIP());

	return true;
}

bool ReadTemp(DS18B20 &result)
{
		byte i;
		byte present = 0;
		byte type_s;
		byte data[12];
		byte addr[8];
		float celsius;

		if (!ds.search(addr)) {
			//Serial.println("No more addresses.");
			//Serial.println();
			ds.reset_search();
			//delay(250);
			return false;
		}

		if (OneWire::crc8(addr, 7) != addr[7]) {
			//Serial.println("CRC is not valid!");
			return false;
		}
		//Serial.println();

		// the first ROM byte indicates which chip
		switch (addr[0]) {
		case 0x10:
			//Serial.println("  Chip = DS18S20");  // or old DS1820
			type_s = 1;
			break;
		case 0x28:
			//Serial.println("  Chip = DS18B20");
			type_s = 0;
			break;
		case 0x22:
			//Serial.println("  Chip = DS1822");
			type_s = 0;
			break;
		default:
			//Serial.println("Device is not a DS18x20 family device.");
			return false;
		}

		ds.reset();
		ds.select(addr);
		ds.write(0x44, 1);        // start conversion, with parasite power on at the end
		delay(750);
		present = ds.reset();
		ds.select(addr);
		ds.write(0xBE);

		for (i = 0; i < 9; i++) {           // we need 9 bytes
			data[i] = ds.read();
			//Serial.print(data[i], HEX);
			//Serial.print(" ");
		}

		int16_t raw = (data[1] << 8) | data[0];
		if (type_s) {
			raw = raw << 3; // 
			//Serial.println("9 bit resolution default");
			if (data[7] == 0x10) {
				//Serial.println("count remain gives full 12 bit resolution");
				raw = (raw & 0xFFF0) + 12 - data[6];
			}
		}
		else {
			//Serial.println("Type = 0");
			byte cfg = (data[4] & 0x60);
			// at lower res, the low bits are undefined, so let's zero them
			if (cfg == 0x00) {
				raw = raw & ~7;  // 
				//Serial.println("9 bit resolution, 93.75 ms");
			}
			else if (cfg == 0x20) {
				raw = raw & ~3; // 
				//Serial.println("10 bit res, 187.5 ms");
			}
			else if (cfg == 0x40) {
				raw = raw & ~1; // 
				//Serial.println("11 bit res, 375 ms");
			}
			else
			{
				////Serial.println("default is 12 bit resolution, 750 ms conversion time");
			}
			//// 
		}
		celsius = (float)raw / 16.0;

		char buffer[17];
		buffer[16] = 0;
		for (int j = 0; j < 8; j++)
			sprintf(&buffer[2 * j], "%02X", addr[j]);

		result.addr = String(buffer);
		result.temp = celsius;

		return true;
}

bool SendToRPi(DS18B20 &result)
{
	const char* host = "raspberrypi";
	const int httpPort = 80;

	// Use WiFiClient class to create TCP connections
	WiFiClient client;
	if (!client.connect(host, httpPort)) {
		Serial.println("> connection failed");
		return false;
	}

	// We now create a URI for the request
	String url = "/espaddtemp.php?";
	url += "dev=";
	url += result.addr;
	url += "&";
	url += "temp=";
	url += result.temp;

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
	}
	client.stop();

	Serial.print("SendToRPi: ");
	Serial.println(line);

	return true;
}

void setup() 
{
	Serial.begin(115200);
	delay(1000);
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	setupMQTT();

	bool known = ConnectToKnownNetwork();
	if (known) 
	{
		if (OTA.check())
			OTA.init();
	}
	//else
	//{
	//	HostServerConfig();
	//}
}

void loop() 
{

	if (OTA.enabled)
	{
		OTA.loop();
	}
	else if (sendTemp)
	{
		DS18B20 result;
		while(ReadTemp(result))
		{
			Serial.print("Dev = ");
			Serial.println(result.addr);
			Serial.print("Temp = ");
			Serial.println(result.temp);

			// TODO: send temperature to RPi
			//SendToRPi(result);
			
			if (!client.connected()) {
				reconnect(result);
			}
			client.loop();
		}

		ulong  b = sleep * 1e6 - millis();
		Serial.print("Going deep sleep ");
		Serial.println(b);

		ESP.deepSleep(b);
		delay(100);
	}

}
