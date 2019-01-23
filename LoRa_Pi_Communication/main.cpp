#define DEBUG


#include <iostream>
#include <string>
#include "LoRaClass.h"
#include "JSON.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"
#define USER_CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/user_config.json"


using namespace std;
using namespace rapidjson;


int main(void)
{
	JSON config;

	JSON user_config;

	if (!config.open(CONFIG_PATH)) {
#ifdef DEBUG
		cout << "config.json could not be found." << endl;
#endif
		exit(EXIT_FAILURE);
	}

	user_config.setPath(USER_CONFIG_PATH);

	user_config.setSPI(5);
	user_config.saveJSON();

	

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
	LoRa.setTxPower(config.getPower(), config.getPAboostPin());

	//Begin LoRa
	LoRa.begin();

	int counter;
	string buffer;

	

	while (true) 
	{
		

		// send packet
		buffer.append("Hello World ");
		buffer.append(to_string(counter));

		cout << buffer << endl;

		LoRa.beginPacket();
		LoRa.print(buffer);
		LoRa.endPacket();

		counter++;
		buffer.clear();
		delay(5000);
	}

	LoRa.end();
	
	

	string i;
	cin >> i;
	return 0;
}