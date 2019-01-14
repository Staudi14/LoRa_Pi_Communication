#include <iostream>
#include <string>
#include "LoRaClass.h"
#include "JSON.h"



#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"


using namespace std;

void onReceive(int packetSize) {
	// received a packet
	cout << ("Received packet '");

	// read packet
	for (int i = 0; i < packetSize; i++) {
		cout << ((char)LoRa.read());
	}

	// print RSSI of packet
	cout << ("' with RSSI ");
	cout << LoRa.packetRssi() << endl;
}

int main(void)
{
	JSON config;

	if (!config.open(CONFIG_PATH)) {
		cout << "config.json could not be found." << endl;
		exit(EXIT_FAILURE);
	}

	/*
	//Testing JSON.h
	cout << "spi: " << config.getSPI() << endl;
	cout << "spi_frequency: " << config.getSPI_frequency() << endl;
	cout << "ss_pin: " << config.getSS_pin() << endl;
	cout << "reset_pin: " << config.getResetPin() << endl;
	cout << "dio0_pin: " << config.getDIO0_pin() << endl;
	cout << "frequency: " << config.getFrequency() << endl;
	cout << "power: " << config.getPower() << endl;
	cout << "rfo_pin: " << config.getRFO_pin() << endl;
	cout << "pa_boost_pin: " << config.getPAboostPin() << endl;
	cout << "mode: " << config.getMode() << endl;
	*/

	//Setting up SPI
	LoRa.setSPIPort(config.getSPI());
	LoRa.setSPIFrequency(config.getSPI_frequency());

	//Set transmission frequency
	LoRa.setFrequency(config.getFrequency());

	//Set Pins
	LoRa.setPins(config.getSS_pin(), config.getResetPin(), config.getDIO0_pin());

	//Set Tx power and RFO pin
	LoRa.setTxPower(config.getPower(), config.getRFO_pin());

	//Begin LoRa
	LoRa.begin();

	LoRa.onReceive(onReceive);
	LoRa.receive();

	while (true);

	LoRa.end();
	string i;
	cin >> i;
	return 0;
}

