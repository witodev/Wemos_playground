// 
// 
// 

#include "MQTT.h"

void MQTTClass::callback(char * topic, byte * payload, unsigned int length)
{
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

//void ShowAnswer(char * topic, byte * payload, unsigned int length)
//{
//	Serial.print("Message arrived [");
//	Serial.print(topic);
//	Serial.print("] ");
//	for (int i = 0; i < length; i++) {
//		Serial.print((char)payload[i]);
//	}
//	Serial.println();
//
//	// Switch on the LED if an 1 was received as first character
//	if ((char)payload[0] == '1') {
//		digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
//										  // but actually the LED is on; this is because
//										  // it is acive low on the ESP-01)
//	}
//	else {
//		digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
//	}
//}

bool MQTTClass::check()
{
	enabled = false;
	WiFiClient client;
	const int httpPort = 80;
	if (!client.connect(Ustawienia.mqtt_host, httpPort)) {
		Serial.println("> MQTT host not avaliable");
		return false;
	}
	else
	{
		enabled = true;
		client.stop();
		return true;
	}
}

void MQTTClass::init()
{
	Serial.println("> MQTT init");
	
	const char* mqtt_host = Ustawienia.mqtt_host;

	client.setClient(espClient);
	client.setServer(mqtt_host, 1883);
	
	using namespace std::placeholders;

	//client.setCallback(ShowAnswer);
	client.setCallback(std::bind(&MQTTClass::callback, this, _1, _2, _3));
}

void MQTTClass::loop()
{
	DS18B20 sensor(D7);
	auto temp = sensor.GetJsonData();
	while (temp != NULL)
	{
		MQTT.Send("event", temp);
		temp = sensor.GetJsonData();
	}
	int sleep = 60;
	auto timeToSleep = sleep * 1e6 - micros() + 1400 * 1e3;
	Serial.print("Sleep time: ");
	Serial.println(timeToSleep);
	ESP.deepSleep(timeToSleep);
}

void MQTTClass::Send(const char * topic, const char * msg)
{
	const char* mqtt_user = Ustawienia.mqtt_user;
	const char* mqtt_pass = Ustawienia.mqtt_pass;
	
	if (!client.connected()) {
		Serial.print("> MQTT conntecting ... ");
		// Attempt to connect
		if (client.connect("esp", mqtt_user, mqtt_pass)) {
			Serial.println("connected");
			//// Once connected, publish an announcement...
			//client.publish(topic, msg);
			//// ... and resubscribe
			////client.subscribe("event");
		}
		else {
			Serial.print("failed, rc=");
			Serial.println(client.state());
			return;
		}
	}
	//else
	//{
	//	Serial.println("> MQTT Connected");
	//	client.publish(topic, msg);
	//}

	client.publish(topic, msg);

	Serial.print("> MQTT msg send: ");
	Serial.println(msg);
}


MQTTClass MQTT;

