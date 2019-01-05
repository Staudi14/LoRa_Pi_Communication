#include <iostream>
#include <fstream>
#include <string>
#include "LoRaClass.h"
#include "rapidjson/document.h"

#define CONFIG_PATH "/home/pi/projects/LoRa_Pi_Communication/config.json"


using namespace std;


int main(void)
{
	string config;
	//Reading in the config File
	fstream fconfig;
	fconfig.open(CONFIG_PATH, ios::in);

	if (fconfig.is_open()) {
		config.assign(istreambuf_iterator<char>(fconfig), istreambuf_iterator<char>());
		cout << config << endl;
	}




	LoRa.begin();	

	LoRa.end();
	string i;
	cin >> i;
	return 0;
}