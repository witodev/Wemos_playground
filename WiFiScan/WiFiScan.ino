/*
*  This sketch demonstrates how to scan WiFi networks.
*  The API is almost the same as with the WiFi Shield library,
*  the most obvious difference being the different file you need to include:
*/

#include <ESP8266WiFi.h>

struct Network {
	String name;
	int32_t pwr;
};

void quickSort(Network arr[], int left, int right);

void setup() {
	Serial.begin(115200);

	// Set WiFi to station mode and disconnect from an AP if it was previously connected
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	delay(100);

	Serial.println("Setup done");
}

void loop() {
	Serial.println("scan start");

	// WiFi.scanNetworks will return the number of networks found
	int n = WiFi.scanNetworks();
	Serial.println("scan done");
	
	Network* networks = new Network[n];

	if (n == 0)
		Serial.println("no networks found");
	else
	{
		Serial.print(n);
		Serial.println(" networks found");
		for (int i = 0; i < n; ++i)
		{						
			networks[i] = { WiFi.SSID(i), WiFi.RSSI(i) };

			delay(10);
		}
		
		quickSort(networks, 0, n);
		
		for (int i = 0; i < n; ++i)
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
	Serial.println("");
	// Wait a bit before scanning again
	delete[] networks;
	delay(5000);
}

// base on http://forum.arduino.cc/index.php?topic=38025.0
void quickSort(Network arr[], int left, int right) {
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

