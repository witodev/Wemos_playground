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

		//for (int i = 0; i < n; ++i)
		//{
		//	// Print SSID and RSSI for each network found
		//	Serial.print(i + 1);
		//	Serial.print(": ");
		//	Serial.print(networks[i].name);
		//	Serial.print(" (");
		//	Serial.print(networks[i].pwr);
		//	Serial.println(")");
		//}
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

void ConfigServerClass::init()
{
	networks = NULL;
	Serial.println("> Server init");
}

void ConfigServerClass::Scan()
{
	FindNetworks();
}


ConfigServerClass ConfigServer;

