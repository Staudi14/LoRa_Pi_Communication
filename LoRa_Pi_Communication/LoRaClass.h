#pragma once

#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

using namespace std;

#define LORA_DEFAULT_SPI			0
#define LORA_DEFAULT_SPI_FREQUENCY	8E6 
#define LORA_DEFAULT_SS_PIN			22
#define LORA_DEFAULT_RESET_PIN		27
#define LORA_DEFAULT_DIO0_PIN		17

#define LORA_DEFAULT_FREQUENCY		868e6
#define LORA_DEFAULT_POWER			17					//17dBm

#define PA_OUTPUT_RFO_PIN			0
#define PA_OUTPUT_PA_BOOST_PIN		5


class LoRaClass
{
public:
	LoRaClass();
	~LoRaClass();

	LoRaClass(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN, 
		long frequency = LORA_DEFAULT_FREQUENCY, int spi = LORA_DEFAULT_SPI, long spi_frequency = LORA_DEFAULT_SPI_FREQUENCY, 
		int power = LORA_DEFAULT_POWER);

	int beginPacket(int implicitHeader = false);
	int endPacket(bool async = false);

	int parsePacket(int size = 0);
	int packetRssi();
	float packetSnr();
	long packetFrequencyError();

	
	virtual size_t write(unsigned int byte);
	virtual size_t write(const unsigned int *buffer, int size);

	virtual int available();
	virtual int read();
	virtual int peek();
	virtual void flush();

	void onReceive(void(*callback)(int));

	void receive(int size = 0);

	void idle();
	void sleep();

	void setTxPower(int level, int outputPin = PA_OUTPUT_PA_BOOST_PIN);
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

	void setOCP(int mA); // Over Current Protection control

	char random();

	void setPins(int ss = LORA_DEFAULT_SS_PIN, int reset = LORA_DEFAULT_RESET_PIN, int dio0 = LORA_DEFAULT_DIO0_PIN);
	void setSPI(SPIClass& spi);
	void setSPIFrequency(int frequency);

	void dumpRegisters(Stream& out);

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
};

