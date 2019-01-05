#pragma once
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

class JSON
{
public:
	JSON();
	~JSON();

private:
	string config;
	string path;

public:
	bool setPath(string fpath);
	bool setPath(const char *fpath);

	string getConfig();

	

};

