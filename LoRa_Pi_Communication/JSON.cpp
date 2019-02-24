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
#include <QtGlobal>


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

bool JSON::setPath(std::string fpath)
{
    std::fstream fconfig;
    fconfig.open(fpath.c_str(), std::ios::in);

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
    std::fstream fconfig;
    fconfig.open(fpath, std::ios::in);

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

        qInfo("config: %s loaded", fpath.c_str());

		return true;
	}
	else {

        qWarning("config: %s not found", fpath.c_str());
		return false;
	}
}

bool JSON::open(const char * fpath)
{
	if (setPath(fpath)) {
		getConfig();

        qInfo("config: %s loaded", fpath);

		return true;
	}
	else {
        qWarning("config: %s not found", fpath);
		return false;
	}

}

std::string JSON::getConfig()
{
    std::fstream fconfig;
    fconfig.open(path, std::ios::in);
	
	config.clear();
	

	if (fconfig.is_open()) {		//Testing if path is correct

        config.assign(std::istreambuf_iterator<char>(fconfig), std::istreambuf_iterator<char>());
		
#ifdef DEBUG
       std::cout << path.c_str() << std::endl;
       std::cout << config.c_str() << std::endl;				//For debugging purposes
#endif
		fconfig.close();

		if (config.length() > 2)
		{
			commentRemover();
		}		
		
		conf.Parse(config.c_str());

        return std::string(config);
	}
	else
	{
        qCritical("config: %s not valid", path.c_str());
        return nullptr;
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

#ifdef DEBUG
            std::cout << "spi is not int" << std::endl;
#endif

            qFatal("config: %s, spi is not int", path.c_str());
		}

	}
	else
	{
#ifdef DEBUG
        std::cout << "spi setting is missing" << std::endl;
#endif
        qFatal("config: %s, spi is missing", path.c_str());
	}
    //return 0;
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
#ifdef DEBUG
            std::cout << "spi_frequency is not int64" << std::endl;
#endif
            qFatal("config: %s, spi_frequency is not long int (int64)", path.c_str());
		}

	}
	else
	{
#ifdef DEBUG
        std::cout << "spi_frequency setting is missing" << std::endl;
#endif
        qFatal("config: %s, spi_frequency is missing", path.c_str());

	}
    //return 0;
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
#ifdef DEBUG
            std::cout << "ss_pin is not int" << std::endl;
#endif
            qFatal("config: %s, ss_pin is not int", path.c_str());
		}

	}
	else
	{
#ifdef DEBUG
        std::cout << "ss_pin setting is missing" << std::endl;
#endif
        qFatal("config: %s, ss_pin is missing", path.c_str());

	}
    //return 0;
}

//TODO
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
            std::cout << "reset_pin is not int" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "reset_pin setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "dio0_pin is not int" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "dio0_pin setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "frequency is not int64" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "frequency setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "power is not int" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "power setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "rfo_pin is not int" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "rfo_pin setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "pa_boost_pin is not bool" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "pa_boost_pin setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    //return 0;
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
            std::cout << "mode is not string" << std::endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
        std::cout << "mode setting is missing" << std::endl;
		exit(EXIT_FAILURE);

	}
    return nullptr;
}

void JSON::setSPI(int spi)
{
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
        std::cout << "SPI: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("spi", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setSPI_frequency(long int frequency)
{
	if (conf.HasMember("spi_frequency"))											
	{
		if (conf["spi_frequency"].IsInt64())
		{
			conf["spi_frequency"].SetInt64(frequency);
		}
	}
	else																
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(frequency);

#ifdef DEBUG
        std::cout << "spi_frequency: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("spi_frequency", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setSS_pin(int ss)
{
	if (conf.HasMember("ss_pin"))
	{
		if (conf["ss_pin"].IsInt())
		{
			conf["ss_pin"].SetInt(ss);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(ss);

#ifdef DEBUG
        std::cout << "ss_pin: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("ss_pin", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setResetPin(int reset)
{
	if (conf.HasMember("reset_pin"))
	{
		if (conf["reset_pin"].IsInt())
		{
			conf["reset_pin"].SetInt(reset);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(reset);

#ifdef DEBUG
        std::cout << "reset_pin: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("reset_pin", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setDIO0_pin(int dio0)
{
	if (conf.HasMember("dio0_pin"))
	{
		if (conf["dio0_pin"].IsInt())
		{
			conf["dio0_pin"].SetInt(dio0);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(dio0);

#ifdef DEBUG
        std::cout << "dio0_pin: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("dio0_pin", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setFrequency(long int frequency)
{
	if (conf.HasMember("frequency"))
	{
		if (conf["frequency"].IsInt64())
		{
			conf["frequency"].SetInt64(frequency);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt64(frequency);

#ifdef DEBUG
        std::cout << "frequency: " << spiSetter.GetInt64() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("frequency", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setPower(int power)
{
	if (conf.HasMember("power"))
	{
		if (conf["power"].IsInt())
		{
			conf["power"].SetInt(power);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(power);

#ifdef DEBUG
        std::cout << "power: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("power", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setRFO_pin(int rfo)
{
	if (conf.HasMember("rfo_pin"))
	{
		if (conf["rfo_pin"].IsInt())
		{
			conf["rfo_pin"].SetInt(rfo);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetInt(rfo);

#ifdef DEBUG
        std::cout << "rfo_pin: " << spiSetter.GetInt() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("rfo_pin", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setPAboostPin(bool paBoost)
{
	if (conf.HasMember("pa_boost_pin"))
	{
		if (conf["pa_boost_pin"].IsBool())
		{
			conf["pa_boost_pin"].SetBool(paBoost);
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetBool(paBoost);

#ifdef DEBUG
        std::cout << "pa_boost_pin: " << spiSetter.GetBool() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("pa_boost_pin", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

void JSON::setMode(std::string mode)
{
	if (conf.HasMember("mode"))
	{
		if (conf["mode"].IsString())
		{
			conf["mode"].SetString(mode.c_str(), conf.GetAllocator());
		}
	}
	else
	{
		rapidjson::Value spiSetter;
		spiSetter.SetString(mode.c_str(), conf.GetAllocator());

#ifdef DEBUG
        std::cout << "mode: " << spiSetter.GetString() << std::endl;
#endif // DEBUG

		conf.GetObject().AddMember("mode", spiSetter, conf.GetAllocator());
	}

	saveJSON();
}

bool JSON::hasSPI()
{
	return conf.HasMember("spi");
}

bool JSON::hasSPI_frequency()
{
	return conf.HasMember("spi_frequency");
}

bool JSON::hasSS()
{
	return conf.HasMember("ss_pin");
}

bool JSON::hasReset()
{
	return conf.HasMember("reset_pin");
}

bool JSON::hasDIO0()
{
	return conf.HasMember("dio0_pin");
}

bool JSON::hasFrequency()
{
	return conf.HasMember("frequency");
}

bool JSON::hasPower()
{
	return conf.HasMember("power");
}

bool JSON::hasRFO()
{
	return conf.HasMember("rfo_pin");
}

bool JSON::hasPAboost()
{
	return conf.HasMember("pa_boost_pin");
}

bool JSON::hasMode()
{
	return conf.HasMember("mode");
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
        std::cout << buffer.GetString() << std::endl;
#endif
		output.open(path.c_str(), std::ios::out | std::ios::trunc);

		output << buffer.GetString();

		output.close();
	}
    catch (const std::ifstream::failure&)
	{
#ifdef DEBUG
        std::cout << "File not found: " << path.c_str() << std::endl;
#endif
		throw;												//Pass exception to caller
	}
	catch (const std::exception&)
	{
#ifdef DEBUG
        std::cout << "Something went wrong with: " << path.c_str() << std::endl;
#endif
		throw;												//Pass exception to caller
	}
}
