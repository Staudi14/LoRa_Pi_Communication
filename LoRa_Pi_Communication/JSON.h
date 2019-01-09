#pragma once
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"
#include "LoRaClass.h"




class JSON
{
public:
	JSON();
	~JSON();

private:
	std::string config;
	std::string path;
	rapidjson::Document conf;

private:
	void commentRemover();


public:
	bool setPath(std::string fpath);
	bool setPath(const char *fpath);

	bool open(std::string fpath);
	bool open(const char *fpath);

	string getConfig();
	
	int getSPI();
	long int getSPI_frequency();
	int getSS_pin();
	int getResetPin();
	int getDIO0_pin();
	long int getFrequency();
	int getPower();
	int getRFO_pin();
	int getPAboostPin();
	std::string getMode();
	

};

