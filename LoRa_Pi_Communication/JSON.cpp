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
	string newString;

	sprintf(iter, "%s", config.c_str());
	
	for (int i = 0; i < config.length() - 1; i++) {
		if (iter[i] == '/' && iter[i + 1] == '/') {
			copy = false;
		}

		if (iter[i] == '\n') {
			copy = true;
			newString.append("\n");
			continue;
		}

		if (copy) {
			newString.append(1,iter[i]);
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
		//cout << config << endl;				//For debugging purposes

		fconfig.close();

		commentRemover();

		
		return string(config);
	}
	else
	{
		return NULL;
	}
}


