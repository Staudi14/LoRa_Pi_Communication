#include <iostream>
#include "LoRaClass.h"

using namespace std;


int main(void)
{
	int packetSize;

	LoRa.setSPIFrequency(1E6);
	LoRa.begin();

	cout << "Waiting on Data" << endl;

	while (true)
	{
		// try to parse packet
		packetSize = LoRa.parsePacket();
		if (packetSize) {
			// received a packet
			cout << ("Received packet '");

			// read packet
			while (LoRa.available()) {
				cout << ((char)LoRa.read());
			}

			// print RSSI of packet
			cout << ("' with RSSI ");
			cout << (LoRa.packetRssi()) << endl;
		}
	}

	LoRa.end();
	return 0;
}