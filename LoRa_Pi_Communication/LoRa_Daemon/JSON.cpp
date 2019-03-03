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
#include <QFile>
#include <QMutexLocker>
#include <QtGlobal>
#include <exception>
#include <sstream>

#define JSPI "spi"
#define JSPI_FREQUENCY "spi_frequency"
#define JFREQUENCY "frequency"
#define JSS_PIN "ss_pin"
#define JRESE_PIN "reset_pin"
#define JDIO0_PIN "dio0_pin"
#define JRFO_PIN "rfo_pin"
#define JPA_BOOST_PIN "pa_boost_pin"
#define JPOWER "power"
#define JMODE "mode"

//#define DEBUG

namespace JSON {
JSON::JSON() { conf.SetObject(); }

JSON::~JSON() {}

std::string JSON::commentRemover(std::string cnf) {

    bool copy = true;
    std::string newString;

    for (unsigned int i = 0; i < cnf.length() - 1; i++) {
        if (cnf.c_str()[i] == '/' && cnf.c_str()[i + 1] == '/') {
            copy = false;
        }

        if (cnf.c_str()[i] == '\n') {
            copy = true;
            newString.append("\n");
            continue;
        }

        if (copy) {
            newString.append(1, cnf.c_str()[i]);
        }
    }

    if (copy) {
        newString.append(1, cnf.c_str()[cnf.length() - 1]);
    }

    return newString;
}

bool JSON::setPath(std::string fpath) { return setPath(fpath.c_str()); }

bool JSON::setPath(const char *fpath) {
    QFile cnf(fpath);

    if (cnf.exists()) { // Testing if path is correct
        path.assign(fpath);
        return true;
    } else {
        return false;
    }
}

bool JSON::exists(){
    QFile cnf(path.c_str());
    return cnf.exists();
}

bool JSON::open(std::string fpath) { return open(fpath.c_str()); }

bool JSON::open(const char *fpath) {
    if (setPath(fpath)) {

        try {
            parseConfig(getConfig());
        } catch (std::exception e) {
            throw;
        }

        qInfo("config: %s loaded", fpath);

        return true;
    } else {
        qWarning("config: %s not found", fpath);
        return false;
    }
}

std::string JSON::getConfig() {

    static QMutex mutex;
    std::fstream fconfig;
    std::string cnf;

    QMutexLocker locker(&mutex);

    fconfig.open(path, std::ios::in);

    if (fconfig.is_open()) { // Testing if path is correct

        cnf.assign(std::istreambuf_iterator<char>(fconfig),
                   std::istreambuf_iterator<char>());

#ifdef DEBUG
        std::cout << path.c_str() << std::endl;
        std::cout << cnf.c_str() << std::endl; // For debugging purposes
#endif
        fconfig.close();
        locker.unlock();

        if (cnf.length() > 2) {
            cnf.assign(commentRemover(cnf));

            return std::string(cnf);
        } else {
            std::stringstream ss;
            ss << path << " is empty";
            throw configuration_empty(ss.str().c_str());
        }

    } else {
        qCritical("config: %s not openable", path.c_str());

        std::stringstream ss;
        ss << path << " is not openable";
        throw configuration_nonexistent(ss.str().c_str());
    }
}

void JSON::parseConfig(std::string cnf) {
    QMutexLocker lockerConf(&mutexConf);
    conf.Parse(cnf.c_str());
}

std::string JSON::getJSON(const char *key, TYPES type) {
    QMutexLocker lockConf(&mutexConf);

    if (conf.HasMember(key)) {
        switch (type) {
        case STRING:
            if (conf[key].IsString()) {
                return std::string(conf[key].GetString());
            } else {
                qFatal("config: %s, %s is not String", path.c_str(), key);
            }
            break;

        case INT:
            if (conf[key].IsInt()) {

                return std::to_string(conf[key].GetInt());
            } else {
                qFatal("config: %s, %s is not Int", path.c_str(), key);
            }
            break;

        case INT64:
            if (conf[key].IsInt64()) {

                return std::to_string(conf[key].GetInt64());
            } else {
                qFatal("config: %s, %s is not Int64", path.c_str(), key);
            }
            break;

        case FLOAT:
            if (conf[key].IsFloat()) {

                return std::to_string(conf[key].GetFloat());
            } else {
                qFatal("config: %s, %s is not Float", path.c_str(), key);
            }
            break;

        case BOOL:
            if (conf[key].IsBool()) {

                return conf[key].GetBool() == true ? std::string("true")
                                                   : std::string("false");
            } else {
                qFatal("config: %s, %s is not Bool", path.c_str(), key);
            }
            break;
        }
    } else {
        throw std::invalid_argument("key is missing");
    }
    return nullptr;
}

int JSON::getSPI() { return std::stoi(getJSON(JSPI, TYPES::INT)); }

long int JSON::getSPI_frequency() {
    return std::stol(getJSON(JSPI_FREQUENCY, TYPES::INT64));
}

int JSON::getSS_pin() { return std::stoi(getJSON(JSS_PIN, TYPES::INT)); }

int JSON::getResetPin() { return std::stoi(getJSON(JRESE_PIN, TYPES::INT)); }

int JSON::getDIO0_pin() { return std::stoi(getJSON(JDIO0_PIN, TYPES::INT)); }

long int JSON::getFrequency() {
    return std::stol(getJSON(JFREQUENCY, TYPES::INT64));
}

int JSON::getPower() { return std::stoi(getJSON(JPOWER, TYPES::INT)); }

int JSON::getRFO_pin() { return std::stoi(getJSON(JRFO_PIN, TYPES::INT)); }

bool JSON::getPAboostPin() {
    return getJSON(JPA_BOOST_PIN, TYPES::BOOL) == "true"? true : false;
}

std::string JSON::getMode() { return getJSON(JMODE, TYPES::STRING); }

void JSON::setJSON(const char *key, std::string value, TYPES type) {
    QMutexLocker lockConf(&mutexConf);

    switch (type) {
    case STRING:
        if (conf.HasMember(key)) {
            if (conf[key].IsString()) {
                conf[key].SetString(value.c_str(), conf.GetAllocator());
            }
        } else {
            rapidjson::Value Vvalue;
            rapidjson::Value Vkey(key, conf.GetAllocator());

            Vvalue.SetString(value.c_str(), conf.GetAllocator());

#ifdef DEBUG
            std::cout << key << " : " << Vvalue.GetString() << std::endl;
#endif // DEBUG

            conf.GetObject().AddMember(Vkey, Vvalue, conf.GetAllocator());
        }
        break;

    case INT:
        if (conf.HasMember(key)) {
            if (conf[key].IsInt()) {
                conf[key].SetInt(std::stoi(value));
            }
        } else {
            rapidjson::Value Vvalue;
            rapidjson::Value Vkey(key, conf.GetAllocator());

            Vvalue.SetInt(std::stoi(value));

#ifdef DEBUG
            std::cout << key << " : " << Vvalue.GetInt() << std::endl;
#endif // DEBUG

            conf.GetObject().AddMember(Vkey, Vvalue, conf.GetAllocator());
        }
        break;

    case INT64:
        if (conf.HasMember(key)) {
            if (conf[key].IsInt64()) {
                conf[key].SetInt64(std::stol(value));
            }
        } else {
            rapidjson::Value Vvalue;
            rapidjson::Value Vkey(key, conf.GetAllocator());

            Vvalue.SetInt64(std::stol(value));

#ifdef DEBUG
            std::cout << key << " : " << Vvalue.GetInt64() << std::endl;
#endif // DEBUG

            conf.GetObject().AddMember(Vkey, Vvalue, conf.GetAllocator());
        }
        break;

    case FLOAT:
        if (conf.HasMember(key)) {
            if (conf[key].IsFloat()) {
                conf[key].SetFloat(std::stof(value));
            }
        } else {
            rapidjson::Value Vvalue;
            rapidjson::Value Vkey(key, conf.GetAllocator());

            Vvalue.SetFloat(std::stof(value));

#ifdef DEBUG
            std::cout << key << " : " << Vvalue.GetFloat() << std::endl;
#endif // DEBUG

            conf.GetObject().AddMember(Vkey, Vvalue, conf.GetAllocator());
        }
        break;

    case BOOL:
        if (conf.HasMember(key)) {
            if (conf[key].IsBool()) {
                conf[key].SetBool(value == "true" ? true : false);
            }
        } else {
            rapidjson::Value Vvalue;
            rapidjson::Value Vkey(key, conf.GetAllocator());

            Vvalue.SetBool(value == "true" ? true : false);

#ifdef DEBUG
            std::cout << key << " : " << Vvalue.GetBool() << std::endl;
#endif // DEBUG

            conf.GetObject().AddMember(Vkey, Vvalue, conf.GetAllocator());
        }
        break;
    }
}

void JSON::setSPI(int spi) { setJSON(JSPI, std::to_string(spi), TYPES::INT); }

void JSON::setSPI_frequency(long int frequency) {
    setJSON(JSPI_FREQUENCY, std::to_string(frequency), TYPES::INT64);
}

void JSON::setSS_pin(int ss) {
    setJSON(JSS_PIN, std::to_string(ss), TYPES::INT);
}

void JSON::setResetPin(int reset) {
    setJSON(JRESE_PIN, std::to_string(reset), TYPES::INT);
}

void JSON::setDIO0_pin(int dio0) {
    setJSON(JDIO0_PIN, std::to_string(dio0), TYPES::INT);
}

void JSON::setFrequency(long int frequency) {
    setJSON(JFREQUENCY, std::to_string(frequency), TYPES::INT);
}

void JSON::setPower(int power) {
    setJSON(JPOWER, std::to_string(power), TYPES::INT);
}

void JSON::setRFO_pin(int rfo) {
    setJSON(JRFO_PIN, std::to_string(rfo), TYPES::INT);
}

void JSON::setPAboostPin(bool paBoost) {
    setJSON(JPA_BOOST_PIN,
            paBoost == true ? std::string("true") : std::string("false"),
            TYPES::BOOL);
}

void JSON::setMode(std::string mode) { setJSON(JMODE, mode, TYPES::STRING); }

bool JSON::hasSPI() { return conf.HasMember(JSPI); }

bool JSON::hasSPI_frequency() { return conf.HasMember(JSPI_FREQUENCY); }

bool JSON::hasSS() { return conf.HasMember(JSS_PIN); }

bool JSON::hasReset() { return conf.HasMember(JRESE_PIN); }

bool JSON::hasDIO0() { return conf.HasMember(JDIO0_PIN); }

bool JSON::hasFrequency() { return conf.HasMember(JFREQUENCY); }

bool JSON::hasPower() { return conf.HasMember(JPOWER); }

bool JSON::hasRFO() { return conf.HasMember(JRFO_PIN); }

bool JSON::hasPAboost() { return conf.HasMember(JPA_BOOST_PIN); }

bool JSON::hasMode() { return conf.HasMember(JMODE); }

void JSON::saveJSON() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    std::fstream output;

    conf.Accept(writer);

    QMutexLocker lockerFile(&mutexFile);

    try {
#ifdef DEBUG
        std::cout << buffer.GetString() << std::endl;
#endif
        output.open(path.c_str(), std::ios::out | std::ios::trunc);

        output << buffer.GetString();

        output.close();
    } catch (const std::ifstream::failure &) {
#ifdef DEBUG
        std::cout << "File not found: " << path.c_str() << std::endl;
#endif
        throw; // Pass exception to caller
    } catch (const std::exception &) {
#ifdef DEBUG
        std::cout << "Something went wrong with: " << path.c_str() << std::endl;
#endif
        throw; // Pass exception to caller
    }
}

} // namespace JSON
