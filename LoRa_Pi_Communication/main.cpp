#include "LoRaClass.h"

using namespace std;


int main(void)
{
	int counter;
	string s = "Hello World";

	//LoRa.setSPIFrequency(1E6);
	LoRa.begin();

	cout << "Waiting on Data" << endl;

	

	while (true)
	{
		cout << ("Sending packet: ");
		cout << (counter)<<endl;

		cout << s <<endl;

		// send packet
		LoRa.beginPacket();
		LoRa.print(s);
		LoRa.endPacket();

		counter++;

		delay(500);
		
	}

	LoRa.end();
	return 0;
}