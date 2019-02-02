/*************************************************************************
This library provides a class to interface with an Semtech SX12xx chip.
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
/*
This Programm uses the BCM pin numbering for the Raspberry Pi
*/
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <sstream>

using namespace std;

//#define DUMPREG										//activates not functioning methode dumpRegesiters

#define WIRINGPI_CODES									//Setup outputs error message

#define LORA_DEFAULT_SPI			0
#define LORA_DEFAULT_SPI_FREQUENCY	8E6 
#define LORA_DEFAULT_SS_PIN			22
#define LORA_DEFAULT_RESET_PIN		27
#define LORA_DEFAULT_DIO0_PIN		17

#define LORA_DEFAULT_FREQUENCY		868e6
#define LORA_DEFAULT_POWER			17					//17dBm

#define PA_OUTPUT_RFO_PIN			0					//represents false
#define PA_OUTPUT_PA_BOOST_PIN		1					//represents true


class LoRaClass
{
public:
	
	LoRaClass();
	
	~LoRaClass();
	
	void begin();

	void end();

	/*LoRaClass(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN, 
		long frequency = LORA_DEFAULT_FREQUENCY, int spi = LORA_DEFAULT_SPI, long spi_frequency = LORA_DEFAULT_SPI_FREQUENCY, 
		int power = LORA_DEFAULT_POWER);*/

	int beginPacket(int implicitHeader = false);
	int endPacket(bool async = false);
	void print(std::string input);

	int parsePacket(uint8_t size = 0);
	int packetRssi();
	float packetSnr();
	long packetFrequencyError();

	
	virtual size_t write(uint8_t byte);
	virtual size_t write(const uint8_t *buffer, size_t size);

	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();

	void onReceive(void(*callback)(int));

	void receive(size_t size = 0);

	void idle();
	void sleep();

	void setTxPower(int level, int outputPin);
	void setTxPower(int level);
	void setFrequency(long frequency);
	void setSpreadingFactor(int sf);
	void setSignalBandwidth(long sbw);
	void setCodingRate4(int denominator);
	void setPreambleLength(long length);
	void setSyncWord(int sw);
	void enableCrc();
	void disableCrc();
	void enableInvertIQ();
	void disableInvertIQ();

	void setOCP(uint8_t mA); // Over Current Protection control

	char random();

	void setPins(int ss, int reset, int dio0);
	void setSPIFrequency(uint32_t frequency);
	void setSPIPort(unsigned int port);

	void setSS(int ss);
	void setReset(int reset);
	void setDIO0(int dio0);

#ifdef DUMPREG
	void dumpRegisters(Stream& out);
#endif

private:
	void explicitHeaderMode();
	void implicitHeaderMode();

	void handleDio0Rise();
	bool isTransmitting();

	int getSpreadingFactor();
	long getSignalBandwidth();

	void setLdoFlag();

	uint8_t readRegister(uint8_t address);
	void writeRegister(uint8_t address, uint8_t value);
	uint8_t singleTransfer(uint8_t address, uint8_t value);

	static void onDio0Rise();

private:
	int _ss;
	int _reset;
	int _dio0;
	long _frequency;
	int _packetIndex;
	int _implicitHeaderMode;
	void(*_onReceive)(int);

	bool _interruptState;
	int _spiFrequency;
	int _spiPort;
	int _power;
	int _pa_rfo_pin;
};

extern LoRaClass LoRa;

