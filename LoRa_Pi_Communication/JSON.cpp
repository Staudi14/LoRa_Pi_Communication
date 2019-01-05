#include "JSON.h"



JSON::JSON()
{
}


JSON::~JSON()
{
}

bool JSON::setPath(string fpath)
{
	fstream fconfig;
	fconfig.open(fpath, ios::in);

	if (fconfig.is_open()) {			//Testing if path is correct
		fconfig.close();
		path = fpath;
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

string JSON::getConfig()
{
	fstream fconfig;
	fconfig.open(path, ios::in);

	if (fconfig.is_open()) {		//Testing if path is correct

		config.assign(istreambuf_iterator<char>(fconfig), istreambuf_iterator<char>());
		cout << config << endl;				//For debugging purposes

		fconfig.close();

		
		return config;
	}
	else
	{
		return NULL;
	}
}


