#include "JSON.h"



JSON::JSON()
{
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
			cout << "pa_boost_pin is not int" << endl;
			exit(EXIT_FAILURE);
		}

	}
	else
	{
		cout << "pa_boost_pin setting is missing" << endl;
		exit(EXIT_FAILURE);

	}
	return NULL;
}

void JSON::setSPI(int spi)
{
	rapidjson::Value spiSetter;
	spiSetter.SetInt(spi);

	conf.AddMember("spi", spiSetter, conf.GetAllocator());
}

void JSON::saveJSON()
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	conf.Accept(writer);

	std::fstream output;
	
	try
	{
		output.open(path.c_str, ios::trunc);

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
		std::out << "Something went wrong with: " + path << endl;
#endif
		throw;												//Pass exception to caller
	}
	


}
