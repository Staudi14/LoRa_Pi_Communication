#include "LoRaClass.h"

// registers
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06

// PA config
#define PA_BOOST                 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40

#define MAX_PKT_LENGTH           255


LoRaClass::LoRaClass()
{
	char version;

	wiringPiSetupGpio();

	//Setup SS Pin
	pinMode(LORA_DEFAULT_SS_PIN, OUTPUT);
	digitalWrite(LORA_DEFAULT_SS_PIN, HIGH);

	//Reset module
	pinMode(LORA_DEFAULT_RESET_PIN, OUTPUT);
	digitalWrite(LORA_DEFAULT_RESET_PIN, LOW);
	delay(10);
	digitalWrite(LORA_DEFAULT_RESET_PIN, HIGH);
	delay(10);

	//SPI
	wiringPiSPISetup(LORA_DEFAULT_SPI, LORA_DEFAULT_SPI_FREQUENCY);			//SPI Mode 0

	//Version check
	version = readRegister(REG_VERSION);								//If version doesn't match terminate the programm and print out an error
	if (version != 0x12)
	{
		cout << "Wrong module version!!" << endl;
		exit(EXIT_FAILURE);
	}

	//put in sleep mode
	sleep();

	//set frequency
	setFrequency(LORA_DEFAULT_FREQUENCY);

	//set base addresses
	writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
	writeRegister(REG_FIFO_RX_BASE_ADDR, 0);

	// set LNA boost
	writeRegister(REG_LNA, readRegister(REG_LNA) | 0x03);

	// set auto AGC
	writeRegister(REG_MODEM_CONFIG_3, 0x04);

	// set output power
	setTxPower(LORA_DEFAULT_POWER);

	// put in standby mode
	idle();
}

LoRaClass::~LoRaClass()
{
	sleep();
}

LoRaClass::LoRaClass(int ss, int reset, int dio0, long frequency, int spi, long spi_frequency, int power)
{
	char version;

	//Setup SS Pin
	pinMode(ss, OUTPUT);
	digitalWrite(ss, HIGH);

	//Reset module
	pinMode(reset, OUTPUT);
	digitalWrite(reset, LOW);
	delay(10);
	digitalWrite(reset, HIGH);
	delay(10);

	//SPI
	wiringPiSPISetup(spi, spi_frequency);			//SPI Mode 0

	//Version check
	version = readRegister(REG_VERSION);			//If version doesn't match terminate the programm and print out an error
	if (version != 0x12)
	{
		cout << "Wrong module version!!" << endl;
		exit(EXIT_FAILURE);
	}

	//put in sleep mode
	sleep();

	//set frequency
	setFrequency(frequency);

	//set base addresses
	writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
	writeRegister(REG_FIFO_RX_BASE_ADDR, 0);

	// set LNA boost
	writeRegister(REG_LNA, readRegister(REG_LNA) | 0x03);

	// set auto AGC
	writeRegister(REG_MODEM_CONFIG_3, 0x04);

	// set output power
	setTxPower(power);

	// put in standby mode
	idle();
}

