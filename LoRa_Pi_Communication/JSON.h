/*************************************************************************
This header file provides a useful class to retrieve data from a json file.
Copyright (C) 2019  Patrik Staudenmayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/
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
	void setSPI_frequency(long int frequency);
	void setSS_pin(int ss);
	void setResetPin(int reset);
	void setDIO0_pin(int dio0);
	void setFrequency(long int frequency);
	void setPower(int power);
	void setRFO_pin(int rfo);
	void setPAboostPin(bool paBoost);
	void setMode(std::string mode);

	//Saving
	void saveJSON();
};


