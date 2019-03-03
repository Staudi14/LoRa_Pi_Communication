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
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <QMutex>
#include <fstream>
#include <iostream>

class JSON {
  public:
    JSON();
    ~JSON();

  private:
    std::string path;
    rapidjson::Document conf;

    // Threadlocker
    QMutex mutexConf;
    QMutex mutexFile;

  private:
    enum TYPES { STRING, INT, INT64, FLOAT, BOOL };

  private:
    std::string commentRemover(std::string);

  public:
    bool setPath(std::string fpath);
    bool setPath(const char *fpath);

    bool open(std::string fpath);
    bool open(const char *fpath);

    std::string getConfig();
    void parseConfig(std::string cnf);

    // Getter functions

    std::string getJSON(const char *key, TYPES type);

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

    // Setter functions

    void setJSON(const char *key, std::string value, TYPES type);

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

    // Assert that parameter exists

    bool hasSPI();
    bool hasSPI_frequency();
    bool hasSS();
    bool hasReset();
    bool hasDIO0();
    bool hasFrequency();
    bool hasPower();
    bool hasRFO();
    bool hasPAboost();
    bool hasMode();

    // Saving
    void saveJSON();
};
