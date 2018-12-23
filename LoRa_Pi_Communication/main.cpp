#include <iostream>
#include "LoRaClass.h"

using namespace std;

void onReceive(int);


int main(void)
{
	int packetSize;

	//LoRa.setSPIFrequency(1E6);
	LoRa.begin();

	cout << "Waiting on Data" << endl;

	// register the receive callback
	LoRa.onReceive(onReceive);

	// put the radio into receive mode
	LoRa.receive();

	while (true)
	{
		
	}

	LoRa.end();
	return 0;
}

void onReceive(int packetSize) {
	// received a packet
	cout << ("Received packet '");

	// read packet
	for (int i = 0; i < packetSize; i++) {
		cout << ((char)LoRa.read());
	}

	// print RSSI of packet
	cout << ("' with RSSI ");
	cout << (LoRa.packetRssi()) << endl;
}