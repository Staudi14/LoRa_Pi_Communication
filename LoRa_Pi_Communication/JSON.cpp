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
#include "JSON.h"
#define DEBUG


JSON::JSON()
{
	conf.SetObject();
}


JSON::~JSON()
{
}

void JSON::commentRemover()
{
	char iter[config.length()];
	bool copy = true;
	std::string newString;

	sprintf(iter, "%s", config.c_str());

	for (unsigned int i = 0; i < config.length() - 1; i++) {
		if (iter[i] == '/' && iter[i + 1] == '/') {
			copy = false;
		}

		if (iter[i] == '\n') {
			copy = true;
			newString.append("\n");
			continue;
		}

		if (copy) {
			newString.append(1, iter[i]);
		}
	}

	if (copy) {
		newString.append(1, iter[config.length() - 1]);
	}

	config = newString;
}

bool JSON::setPath(string fpath)
{
	fstream fconfig;
	fconfig.open(fpath.c_str(), ios::in);

	if (fconfig.is_open()) {		//Testing if path is correct
		fconfig.close();
		path.assign(fpath.c_str());
		return true;
	}
	else
	{
		return false;
	}
}

bool JSON::setPath(const char * fpath)
{
	fstream fconfig;
	fconfig.open(fpath, ios::in);

	if (fconfig.is_open()) {		//Testing if path is correct
		fconfig.close();
		path.assign(fpath);
		return true;
	}
	else
	{
		return false;
	}
}

bool JSON::open(std::string fpath)
{
	if (setPath(fpath)) {
		getConfig();
		return true;
	}
	else {
		return false;
	}
}

bool JSON::open(const char * fpath)
{
	if (setPath(fpath)) {
		getConfig();
		return true;
	}
	else {
		return false;
	}

}

string JSON::getConfig()
{
	fstream fconfig;
	fconfig.open(path, ios::in);
	
	config.clear();
	

	if (fconfig.is_open()) {		//Testing if path is correct

		config.assign(istreambuf_iterator<char>(fconfig), istreambuf_iterator<char>());
		//cout << config << endl;				//For debugging purposes

		fconfig.close();

		commentRemover();

		conf.Parse(config.c_str());


		return string(config);
	}
	else
	{
		return NULL;
	}
}

int JSON::getSPI()
{
	if (conf.HasMember("spi"))
	{
		if (conf["spi"].IsInt())
		{
			return conf["spi"].GetInt();
		}
		else
		{
			cout << "spi is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "spi setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

long int JSON::getSPI_frequency()
{
	if (conf.HasMember("spi_frequency"))
	{
		if (conf["spi_frequency"].IsInt64())
		{
			return (long int)conf["spi_frequency"].GetInt64();
		}
		else
		{
			cout << "spi_frequency is not int64" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "spi_frequency setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getSS_pin()
{
	if (conf.HasMember("ss_pin"))
	{
		if (conf["ss_pin"].IsInt())
		{
			return conf["ss_pin"].GetInt();
		}
		else
		{
			cout << "ss_pin is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "ss_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getResetPin()
{
	if (conf.HasMember("reset_pin"))
	{
		if (conf["reset_pin"].IsInt())
		{
			return conf["reset_pin"].GetInt();
		}
		else
		{
			cout << "reset_pin is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "reset_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getDIO0_pin()
{
	if (conf.HasMember("dio0_pin"))
	{
		if (conf["dio0_pin"].IsInt())
		{
			return conf["dio0_pin"].GetInt();
		}
		else
		{
			cout << "dio0_pin is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "dio0_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

long int JSON::getFrequency()
{
	if (conf.HasMember("frequency"))
	{
		if (conf["frequency"].IsInt64())
		{
			return (long int)conf["frequency"].GetInt64();
		}
		else
		{
			cout << "frequency is not int64" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "frequency setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getPower()
{
	if (conf.HasMember("power"))
	{
		if (conf["power"].IsInt())
		{
			return conf["power"].GetInt();
		}
		else
		{
			cout << "power is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "power setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getRFO_pin()
{
	if (conf.HasMember("rfo_pin"))
	{
		if (conf["rfo_pin"].IsInt())
		{
			return conf["rfo_pin"].GetInt();
		}
		else
		{
			cout << "rfo_pin is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "rfo_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

int JSON::getPAboostPin()
{
	if (conf.HasMember("pa_boost_pin"))
	{
		if (conf["pa_boost_pin"].IsBool())
		{
			return (conf["pa_boost_pin"].GetBool()? 1 : 0);
		}
		else
		{
			cout << "pa_boost_pin is not bool" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "pa_boost_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return 0;
}

std::string JSON::getMode()
{
	if (conf.HasMember("mode"))
	{
		if (conf["mode"].IsString())
		{
			return std::string(conf["mode"].GetString());
		}
		else
		{
			cout << "mode is not string" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "mode setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return NULL;
}

void JSON::setSPI(int spi)
{
	setValue("spi", spi);
/*
	if (conf.HasMember("spi"))											//spi value already exists
	{
		if (conf["spi"].IsInt())
		{
			conf["spi"].SetInt(spi);
		}
	}
	else																//spi value does not exist already and must be created
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(spi);

#ifdef DEBUG
		std::cout << "SPI: " << spiSetter.GetInt() << endl;
#endif // DEBUG

		conf.GetObject().AddMember("spi", spiSetter, conf.GetAllocator());
	}
*/	
}

void JSON::setSPI_frequency(long int frequency)
{
	setValue("spi_frequency", frequency);
}

void JSON::setSS_pin(int ss)
{
	setValue("ss_pin", ss);
}

void JSON::setResetPin(int reset)
{
	setValue("reset_pin", reset);
}

void JSON::setDIO0_pin(int dio0)
{
	setValue("dio0_pin", dio0);
}

void JSON::setFrequency(long int frequency)
{
	setValue("frequency", frequency);
}

void JSON::setPower(int power)
{
	setValue("power", power);
}

void JSON::setRFO_pin(int rfo)
{
	setValue("rfo_pin", rfo);
}

void JSON::setPAboostPin(bool paBoost)
{
	setValue("pa_boost_pin", paBoost);
}

void JSON::setMode(std::string mode)
{
	setValue("mode", mode);
}

void JSON::saveJSON()
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	std::fstream output;

	conf.Accept(writer);

	
	
	try
	{
#ifdef DEBUG
		std::cout << buffer.GetString() << endl;
#endif
		output.open(path.c_str(), std::ios::out | std::ios::trunc);

		output << buffer.GetString();

		output.close();
	}
	catch (const std::ifstream::failure& e)
	{
#ifdef DEBUG
		std::cout << "File not found: " + path << endl;
#endif
		throw;												//Pass exception to caller
	}
	catch (const std::exception&)
	{
#ifdef DEBUG
		std::cout << "Something went wrong with: " + path << endl;
#endif
		throw;												//Pass exception to caller
	}
}

template<class T>
T inline JSON::getValue(std::string name)
{
	if (conf.HasMember(name.c_str()) {
		return conf[name.c_str()].Get();
	}
	else
	{
		throw invalid_argument("name not found in json");
		return 0;
	}
}

template<class T>
T inline JSON::getValue(char *name)
{
	if (conf.HasMember(name) {
		return conf[name].Get();
	}
	else
	{
		throw invalid_argument("name not found in json");
			return 0;
	}
}

template<class T>
inline void JSON::setValue(std::string name, T value)
{
	if (conf.HasMember(name.c_str())														//If value already exists
	{
		if (conf[name.c_str()].IsInt() && std::is_same_v< T , int>)							//In case value is int
		{
			conf[name.c_str()].SetInt(value);
		}
		else if (conf[name.c_str()].IsInt64() && std::is_same_v< T, int64_t>)				//In case value is int64
		{
			conf[name.c_str()].SetInt64(value);
		}
		else if (conf[name.c_str()].IsString() && std::is_same_v< T, std::string>)			//In case value is std::string
		{
			conf[name.c_str()].SetString(value.c_str());
		}
		else if (conf[name.c_str()].IsBool() && std::is_same_v< T, bool>)					//In case value is bool
		{
			conf[name.c_str()].SetBool(value);
		}
		
	}
	else																					//spi value does not exist already and must be created
	{
		rapidjson::Value spiSetter;

#ifdef DEBUG
		std::cout << "Created new Element " + name;
#endif // DEBUG


		if (std::is_same_v< T, int >)
		{
			spiSetter.SetInt(value);
#ifdef DEBUG
			std::cout << " with int Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, int64_t >)
		{
			spiSetter.SetInt64(value);
#ifdef DEBUG
			std::cout << " with int64 Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, std::string >)
		{
			spiSetter.SetString(value.c_str());
#ifdef DEBUG
			std::cout << " with string Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, bool>)
		{
			spiSetter.SetBool(value);
#ifdef DEBUG
			std::cout << " with boolean Value of " + value << std::endl;
#endif // DEBUG
		}

		

		conf.GetObject().AddMember(name.c_str(), spiSetter, conf.GetAllocator());
	}
}

template<class T>
void inline JSON::setValue(const char *name, T value)
{
	if (conf.HasMember(name)														//If value already exists
	{
		if (conf[name].IsInt() && std::is_same_v< T, int>)							//In case value is int
		{
			conf[name].SetInt(value);
		}
		else if (conf[name].IsInt64() && std::is_same_v< T, int64_t>)				//In case value is int64
		{
			conf[name].SetInt64(value);
		}
		else if (conf[name].IsString() && std::is_same_v< T, std::string>)			//In case value is std::string
		{
			conf[name].SetString(value.c_str());
		}
		else if (conf[name].IsBool() && std::is_same_v< T, bool>)					//In case value is bool
		{
			conf[name].SetInt64(value);
		}

	}
	else																					//spi value does not exist already and must be created
	{
		rapidjson::Value spiSetter;

#ifdef DEBUG
		std::cout << "Created new Element " + name;
#endif // DEBUG


		if (std::is_same_v< T, int >)
		{
			spiSetter.SetInt(value);
#ifdef DEBUG
			std::cout << " with int Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, int64_t >)
		{
			spiSetter.SetInt64(value);
#ifdef DEBUG
			std::cout << " with int64 Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, std::string >)
		{
			spiSetter.SetString(value.c_str());
#ifdef DEBUG
			std::cout << " with string Value of " + value << std::endl;
#endif // DEBUG
		}
		else if (std::is_same_v< T, bool>)
		{
			spiSetter.SetBool(value);
#ifdef DEBUG
			std::cout << " with boolean Value of " + value << std::endl;
#endif // DEBUG
		}



		conf.GetObject().AddMember(name, spiSetter, conf.GetAllocator());
	}
}