
#include <FS.h>

char* Convert(const char * source, size_t len = 100)
{
	auto buff = new char[len];
	strcpy(buff, source);
	return buff;
}

char* GetPage(const char * title)
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
		Serial.print("> File size: ");
		Serial.println(size);
		//std::unique_ptr<char[]> buf(new char[size]);
		char* buf = new char[size];
		configFile.readBytes(buf, size);
		SPIFFS.end();

		return buf;
	}
	else
		return NULL;
}

void setup()
{
	Serial.begin(115200);
	Serial.println();
	Serial.println();
	Serial.println("Serial.begin");

}
void loop()
{
	auto index1 = GetPage("indexStart.txt");
	auto index2 = GetPage("indexEnd.txt");

	yield();
	delay(1000);

	Serial.println(index1);
	Serial.println(index2);

	delete index1;
	delete index2;
}
