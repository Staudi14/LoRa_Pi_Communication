#include <iostream>
#include <fstream>
#include <string>
#include "LoRaClass.h"
#include "JSON.h"



#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"


using namespace std;


int main(void)
{
	JSON config;

	if (config.setPath(CONFIG_PATH)) {
		cout << config.getConfig() << endl;


	}
	else {
		cout << "config.json nicht an angegeben Pfad" << endl;
	}
	
	




	LoRa.begin();	

	LoRa.end();
	string i;
	cin >> i;
	return 0;
}