#include <iostream>
#include <string>
#include "LoRaClass.h"
#include "JSON.h"



#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"


using namespace std;


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



	LoRa.begin();

	LoRa.end();
	string i;
	cin >> i;
	return 0;
}