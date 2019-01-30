#pragma once
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
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


	//Getter functions

	template<class T>
	T getValue(std::string name);

	template<class T>
	T getValue(char *name);

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

	//Setter functions

	template<class T>
	void setValue(std::string name, T value);

	template<class T>
	void setValue(const char *name, T value);

	void setSPI(int spi);
	/*void setSPI_frequency(long int frequency);
	void setSS_pin(int ss);
	void setResetPin(int reset);
	void setDIO0_pin(int dio0);
	void setFrequency(long int frequency);
	void setPower(int power);
	void setRFO_pin(int rfo);
	void setPAboostPin(int paBoost);
	void setMode(std::string mode);*/

	//Saving
	void saveJSON();
};


