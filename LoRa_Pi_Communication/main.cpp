#include <iostream>
#include <fstream>
#include <string>
#include "LoRaClass.h"
#include "rapidjson/document.h"

#define CONFIG_PATH "/usr/bin/lora/config.json"


using namespace std;


int main(void)
{
	string config;
	//Reading in the config File
	fstream fconfig;
	fconfig.open(CONFIG_PATH, ios::in);

	if (fconfig.is_open()) {
		fconfig >> config;
		cout << config << endl;
	}




	LoRa.begin();	

	LoRa.end();
	return 0;
}