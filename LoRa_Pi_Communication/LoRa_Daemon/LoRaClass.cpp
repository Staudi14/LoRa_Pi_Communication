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

#include "LoRaClass.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include <QtGlobal>

#define DEBUG // Enable debug messages

// misc
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)                                         \
    (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// registers
#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FRF_MSB 0x06
#define REG_FRF_MID 0x07
#define REG_FRF_LSB 0x08
#define REG_PA_CONFIG 0x09
#define REG_OCP 0x0b
#define REG_LNA 0x0c
#define REG_FIFO_ADDR_PTR 0x0d
#define REG_FIFO_TX_BASE_ADDR 0x0e
#define REG_FIFO_RX_BASE_ADDR 0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS 0x12
#define REG_RX_NB_BYTES 0x13
#define REG_PKT_SNR_VALUE 0x19
#define REG_PKT_RSSI_VALUE 0x1a
#define REG_MODEM_CONFIG_1 0x1d
#define REG_MODEM_CONFIG_2 0x1e
#define REG_PREAMBLE_MSB 0x20
#define REG_PREAMBLE_LSB 0x21
#define REG_PAYLOAD_LENGTH 0x22
#define REG_MODEM_CONFIG_3 0x26
#define REG_FREQ_ERROR_MSB 0x28
#define REG_FREQ_ERROR_MID 0x29
#define REG_FREQ_ERROR_LSB 0x2a
#define REG_RSSI_WIDEBAND 0x2c
#define REG_DETECTION_OPTIMIZE 0x31
#define REG_INVERTIQ 0x33
#define REG_DETECTION_THRESHOLD 0x37
#define REG_SYNC_WORD 0x39
#define REG_INVERTIQ2 0x3b
#define REG_DIO_MAPPING_1 0x40
#define REG_VERSION 0x42
#define REG_PA_DAC 0x4d

// modes
#define MODE_LONG_RANGE_MODE 0x80
#define MODE_SLEEP 0x00
#define MODE_STDBY 0x01
#define MODE_TX 0x03
#define MODE_RX_CONTINUOUS 0x05
#define MODE_RX_SINGLE 0x06

// PA config
#define PA_BOOST 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK 0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK 0x40

#define MAX_PKT_LENGTH 255

LoRaClass::LoRaClass() {
    _ss = LORA_DEFAULT_SS_PIN;
    _reset = LORA_DEFAULT_RESET_PIN;
    _dio0 = LORA_DEFAULT_DIO0_PIN;
    _frequency = LORA_DEFAULT_FREQUENCY;
    _spiFrequency = LORA_DEFAULT_SPI_FREQUENCY;
    _spiPort = LORA_DEFAULT_SPI;
    _power = LORA_DEFAULT_POWER;
    _pa_rfo_pin = PA_OUTPUT_PA_BOOST_PIN;
}

LoRaClass::~LoRaClass() { end(); }

void LoRaClass::begin() {
    char version;

    _interruptState = false;
    _spiFrequency = _spiFrequency;
    _spiPort = _spiPort;

    wiringPiSetupGpio();

    // Setup SS Pin
    pinMode(_ss, OUTPUT);
    digitalWrite(_ss, HIGH);

    // Reset module
    pinMode(_reset, OUTPUT);
    digitalWrite(_reset, LOW);
    delay(10);
    digitalWrite(_reset, HIGH);
    delay(10);

    // SPI
    wiringPiSPISetup(_spiPort, _spiFrequency); // SPI Mode 0

    // Version check
    version = readRegister(REG_VERSION); // If version doesn't match terminate
                                         // the programm and print out an error
    if (version != 0x12) {
        qCritical("lora: Wrong module version");
#ifdef DEBUG
        std::cout << "Wrong module version!!" << std::endl;
#endif

        exit(EXIT_FAILURE);
    }

    // put in sleep mode
    sleep();

    // set frequency
    setFrequency(_frequency);

    // set base addresses
    writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
    writeRegister(REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost
    writeRegister(REG_LNA, readRegister(REG_LNA) | 0x03);

    // set auto AGC
    writeRegister(REG_MODEM_CONFIG_3, 0x04);

    // set output power
    setTxPower(_power, _pa_rfo_pin);

    // put in standby mode
    idle();
}

void LoRaClass::end() {
    sleep();
    char buffer[33];

    sprintf(buffer, "gpio unexportall");

    system(buffer);
}

int LoRaClass::beginPacket(int implicitHeader) {
    if (isTransmitting()) {
        return 0;
    }

    if (_interruptState == true) {
        char buffer[33];

        sprintf(buffer, "gpio edge %d none", _dio0);

        system(buffer);
    }

    // put in standby mode
    idle();

    if (implicitHeader) {
        implicitHeaderMode();
    } else {
        explicitHeaderMode();
    }

    // reset FIFO address and paload length
    writeRegister(REG_FIFO_ADDR_PTR, 0);
    writeRegister(REG_PAYLOAD_LENGTH, 0);

    return 1;
}

int LoRaClass::endPacket(bool async) {
    // put in TX mode
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    if (async) {
        // grace time is required for the radio
        delayMicroseconds(150);
    } else {
        // wait for TX done
        while ((readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0)
            ;

        // clear IRQ's
        writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    if (_interruptState == true) {
        wiringPiISR(_dio0, INT_EDGE_RISING, LoRaClass::onDio0Rise);
    }

    return 1;
}

bool LoRaClass::isTransmitting() {
    if ((readRegister(REG_OP_MODE) & MODE_TX) == MODE_TX) {
        return true;
    }

    if (readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) {
        // clear IRQ's
        writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
    }

    return false;
}

int LoRaClass::parsePacket(uint8_t size) {
    int packetLength = 0;
    uint8_t irqFlags = readRegister(REG_IRQ_FLAGS);

    if (size > 0) {
        implicitHeaderMode();

        writeRegister(REG_PAYLOAD_LENGTH, size & 0xff);
    } else {
        explicitHeaderMode();
    }

    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_RX_DONE_MASK) &&
        (irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
        // received a packet
        _packetIndex = 0;

        // read packet length
        if (_implicitHeaderMode) {
            packetLength = readRegister(REG_PAYLOAD_LENGTH);
        } else {
            packetLength = readRegister(REG_RX_NB_BYTES);
        }

        // set FIFO address to current RX address
        writeRegister(REG_FIFO_ADDR_PTR,
                      readRegister(REG_FIFO_RX_CURRENT_ADDR));

        // put in standby mode
        idle();
    } else if (readRegister(REG_OP_MODE) !=
               (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE)) {
        // not currently in RX mode

        // reset FIFO address
        writeRegister(REG_FIFO_ADDR_PTR, 0);

        // put in single RX mode
        writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
    }

    return packetLength;
}

int LoRaClass::packetRssi() {
    return (readRegister(REG_PKT_RSSI_VALUE) -
            (_frequency < 868E6 ? 164 : 157));
}

float LoRaClass::packetSnr() {
    return ((unsigned char)readRegister(REG_PKT_SNR_VALUE)) * 0.25f;
}

long LoRaClass::packetFrequencyError() {
    int32_t freqError = 0;
    freqError = static_cast<int32_t>(readRegister(REG_FREQ_ERROR_MSB) & 0xB111);
    freqError <<= 8L;
    freqError += static_cast<int32_t>(readRegister(REG_FREQ_ERROR_MID));
    freqError <<= 8L;
    freqError += static_cast<int32_t>(readRegister(REG_FREQ_ERROR_LSB));

    if (readRegister(REG_FREQ_ERROR_MSB) & 0xB1000) { // Sign bit is on
        freqError -= 524288; // B1000'0000'0000'0000'0000
    }

    const float fXtal = 32E6; // FXOSC: crystal oscillator (XTAL) frequency
                              // (2.5. Chip Specification, p. 14)
    const float fError =
        (float)(((float)freqError * (float)(1L << 24)) / (float)fXtal) *
        (float)(getSignalBandwidth() / 500000.0f); // p. 37

    return static_cast<long>(fError);
}

size_t LoRaClass::write(uint8_t byte) { return write(&byte, sizeof(byte)); }

size_t LoRaClass::write(const uint8_t *buffer, size_t size) {
    char currentLength = readRegister(REG_PAYLOAD_LENGTH);

    // check size
    if ((currentLength + size) > MAX_PKT_LENGTH) {
        size = MAX_PKT_LENGTH - currentLength;
    }

    // write data
    for (size_t i = 0; i < size; i++) {
        writeRegister(REG_FIFO, buffer[i]);
    }

    // update length
    writeRegister(REG_PAYLOAD_LENGTH, currentLength + size);

    return size;
}

int LoRaClass::available() {
    return (readRegister(REG_RX_NB_BYTES) - _packetIndex);
}

int LoRaClass::read() {
    if (!available()) {
        return -1;
    }

    _packetIndex++;

    return readRegister(REG_FIFO);
}

int LoRaClass::peek() {
    if (!available()) {
        return -1;
    }

    // store current FIFO address
    uint8_t currentAddress = readRegister(REG_FIFO_ADDR_PTR);

    // read
    uint8_t b = readRegister(REG_FIFO);

    // restore FIFO address
    writeRegister(REG_FIFO_ADDR_PTR, currentAddress);

    return b;
}

void LoRaClass::flush() {}

void LoRaClass::onReceive(
    void (*callback)(int)) // callback wird im handleDI0Rise aufgerufen!!
{ // TODO: wenn transaktion gestartet wird Interrupt beenden!!! danach wieder
    // starten
    _onReceive = callback;

    if (callback) {
        pinMode(_dio0, INPUT);

        writeRegister(REG_DIO_MAPPING_1, 0x00);

        // Set up Pin change interrupt
        wiringPiISR(_dio0, INT_EDGE_RISING, LoRaClass::onDio0Rise);
        _interruptState = true;
    } else {
        char buffer[33];

        sprintf(buffer, "gpio edge %d none", _dio0);

        system(buffer);

        _interruptState = false;
    }
}

void LoRaClass::receive(size_t size) {
    if (size > 0) {
        implicitHeaderMode();

        writeRegister(REG_PAYLOAD_LENGTH, size & 0xff);
    } else {
        explicitHeaderMode();
    }

    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS);
}

void LoRaClass::idle() {
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

void LoRaClass::sleep() {
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}

// Output Pin only true(1) or false(0)
//
void LoRaClass::setTxPower(unsigned int level, int outputPin) {
    _power = level;
    _pa_rfo_pin = outputPin;

    if (PA_OUTPUT_RFO_PIN == _pa_rfo_pin) {
        // RFO
        if (_power > 14) {
            _power = 14;
        }

        writeRegister(REG_PA_CONFIG, 0x70 | level);
    } else {
        // PA BOOST
        if (_power > 17) {
            if (_power > 20) {
                _power = 20;
            }

            // subtract 3 from _power, so 18 - 20 maps to 15 - 17
            _power -= 3;

            // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
            writeRegister(REG_PA_DAC, 0x87);
            setOCP(140);
        } else {
            if (_power < 2) {
                _power = 2;
            }
            // Default value PA_HF/LF or +17dBm
            writeRegister(REG_PA_DAC, 0x84);
            setOCP(100);
        }

        writeRegister(REG_PA_CONFIG, PA_BOOST | (_power - 2));
    }
}

void LoRaClass::setTxPower(unsigned int level) {
    _power = level;

    if (PA_OUTPUT_RFO_PIN == _pa_rfo_pin) {
        // RFO
        if (_power > 14) {
            _power = 14;
        }

        writeRegister(REG_PA_CONFIG, 0x70 | level);
    } else {
        // PA BOOST
        if (_power > 17) {
            if (_power > 20) {
                _power = 20;
            }

            // subtract 3 from _power, so 18 - 20 maps to 15 - 17
            _power -= 3;

            // High Power +20 dBm Operation (Semtech SX1276/77/78/79 5.4.3.)
            writeRegister(REG_PA_DAC, 0x87);
            setOCP(140);
        } else {
            if (_power < 2) {
                _power = 2;
            }
            // Default value PA_HF/LF or +17dBm
            writeRegister(REG_PA_DAC, 0x84);
            setOCP(100);
        }

        writeRegister(REG_PA_CONFIG, PA_BOOST | (_power - 2));
    }
}

void LoRaClass::setFrequency(long frequency) {
    _frequency = frequency;

    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    writeRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
    writeRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
    writeRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

int LoRaClass::getSpreadingFactor() {
    return readRegister(REG_MODEM_CONFIG_2) >> 4;
}

void LoRaClass::setSpreadingFactor(int sf) {
    if (sf < 6) {
        sf = 6;
    } else if (sf > 12) {
        sf = 12;
    }

    if (sf == 6) {
        writeRegister(REG_DETECTION_OPTIMIZE, 0xc5);
        writeRegister(REG_DETECTION_THRESHOLD, 0x0c);
    } else {
        writeRegister(REG_DETECTION_OPTIMIZE, 0xc3);
        writeRegister(REG_DETECTION_THRESHOLD, 0x0a);
    }

    writeRegister(REG_MODEM_CONFIG_2,
                  (readRegister(REG_MODEM_CONFIG_2) & 0x0f) |
                      ((sf << 4) & 0xf0));
    setLdoFlag();
}

long LoRaClass::getSignalBandwidth() {
    char bw = (readRegister(REG_MODEM_CONFIG_1) >> 4);

    switch (bw) {
    case 0:
        return 7.8E3;
    case 1:
        return 10.4E3;
    case 2:
        return 15.6E3;
    case 3:
        return 20.8E3;
    case 4:
        return 31.25E3;
    case 5:
        return 41.7E3;
    case 6:
        return 62.5E3;
    case 7:
        return 125E3;
    case 8:
        return 250E3;
    case 9:
        return 500E3;
    }

    return -1;
}

void LoRaClass::setSignalBandwidth(long sbw) {
    int bw;

    if (sbw <= 7.8E3) {
        bw = 0;
    } else if (sbw <= 10.4E3) {
        bw = 1;
    } else if (sbw <= 15.6E3) {
        bw = 2;
    } else if (sbw <= 20.8E3) {
        bw = 3;
    } else if (sbw <= 31.25E3) {
        bw = 4;
    } else if (sbw <= 41.7E3) {
        bw = 5;
    } else if (sbw <= 62.5E3) {
        bw = 6;
    } else if (sbw <= 125E3) {
        bw = 7;
    } else if (sbw <= 250E3) {
        bw = 8;
    } else /*if (sbw <= 250E3)*/ {
        bw = 9;
    }

    writeRegister(REG_MODEM_CONFIG_1,
                  (readRegister(REG_MODEM_CONFIG_1) & 0x0f) | (bw << 4));
    setLdoFlag();
}

void LoRaClass::setLdoFlag() {
    // Section 4.1.1.5
    long symbolDuration =
        1000 / (getSignalBandwidth() / (1L << getSpreadingFactor()));

    // Section 4.1.1.6
    bool ldoOn = symbolDuration > 16;

    uint8_t config3 = readRegister(REG_MODEM_CONFIG_3);
    bitWrite(config3, 3, ldoOn);
    writeRegister(REG_MODEM_CONFIG_3, config3);
}

void LoRaClass::setCodingRate4(int denominator) {
    if (denominator < 5) {
        denominator = 5;
    } else if (denominator > 8) {
        denominator = 8;
    }

    int cr = denominator - 4;

    writeRegister(REG_MODEM_CONFIG_1,
                  (readRegister(REG_MODEM_CONFIG_1) & 0xf1) | (cr << 1));
}

void LoRaClass::setPreambleLength(long length) {
    writeRegister(REG_PREAMBLE_MSB, (uint8_t)(length >> 8));
    writeRegister(REG_PREAMBLE_LSB, (uint8_t)(length >> 0));
}

void LoRaClass::setSyncWord(int sw) { writeRegister(REG_SYNC_WORD, sw); }

void LoRaClass::enableCrc() {
    writeRegister(REG_MODEM_CONFIG_2, readRegister(REG_MODEM_CONFIG_2) | 0x04);
}

void LoRaClass::disableCrc() {
    writeRegister(REG_MODEM_CONFIG_2, readRegister(REG_MODEM_CONFIG_2) & 0xfb);
}

void LoRaClass::enableInvertIQ() {
    writeRegister(REG_INVERTIQ, 0x66);
    writeRegister(REG_INVERTIQ2, 0x19);
}

void LoRaClass::disableInvertIQ() {
    writeRegister(REG_INVERTIQ, 0x27);
    writeRegister(REG_INVERTIQ2, 0x1d);
}

void LoRaClass::setOCP(uint8_t mA) {
    uint8_t ocpTrim = 27;

    if (mA <= 120) {
        ocpTrim = (mA - 45) / 5;
    } else if (mA <= 240) {
        ocpTrim = (mA + 30) / 10;
    }

    writeRegister(REG_OCP, 0x20 | (0x1F & ocpTrim));
}

char LoRaClass::random() { return readRegister(REG_RSSI_WIDEBAND); }

void LoRaClass::setPins(int ss, int reset, int dio0) {
    _ss = ss;
    _reset = reset;
    _dio0 = dio0;
}

void LoRaClass::setSPIFrequency(uint32_t frequency) {
    wiringPiSPISetup(_spiPort, frequency);
}

#ifdef DUMPREG
// Don't know what it does and doens't get called in any other methode.
void LoRaClass::dumpRegisters(Stream &out) {
    for (int i = 0; i < 128; i++) {
        out.print("0x");
        out.print(i, HEX);
        out.print(": 0x");
        out.println(readRegister(i), HEX);
    }
}
#endif

void LoRaClass::explicitHeaderMode() {
    _implicitHeaderMode = 0;

    writeRegister(REG_MODEM_CONFIG_1, readRegister(REG_MODEM_CONFIG_1) & 0xfe);
}

void LoRaClass::implicitHeaderMode() {
    _implicitHeaderMode = 1;

    writeRegister(REG_MODEM_CONFIG_1, readRegister(REG_MODEM_CONFIG_1) | 0x01);
}

void LoRaClass::handleDio0Rise() {
    int irqFlags = readRegister(REG_IRQ_FLAGS);

    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {
        // received a packet
        _packetIndex = 0;

        // read packet length
        int packetLength = _implicitHeaderMode
                               ? readRegister(REG_PAYLOAD_LENGTH)
                               : readRegister(REG_RX_NB_BYTES);

        // set FIFO address to current RX address
        writeRegister(REG_FIFO_ADDR_PTR,
                      readRegister(REG_FIFO_RX_CURRENT_ADDR));

        if (_onReceive) {
            _onReceive(packetLength);
        }

        // reset FIFO address
        writeRegister(REG_FIFO_ADDR_PTR, 0);
    }
}

uint8_t LoRaClass::readRegister(uint8_t address) {
    return singleTransfer(address & 0x7f, 0x00);
}

void LoRaClass::writeRegister(uint8_t address, uint8_t value) {
    singleTransfer(address | 0x80, value);
}

uint8_t LoRaClass::singleTransfer(uint8_t address, uint8_t value) {
    unsigned char buffer[2] = {address, value};

    digitalWrite(_ss, LOW);

    wiringPiSPIDataRW(_spiPort, buffer, 2);

    digitalWrite(_ss, HIGH);

    return buffer[1];
}

void LoRaClass::onDio0Rise() { LoRa.handleDio0Rise(); }

void LoRaClass::print(std::string input) {

    const char *in = input.c_str();

    for (unsigned int i = 0; i < (input.length() + 1); i++) {
        write(in[i]);
    }
}

void LoRaClass::setSPIPort(unsigned int port) {
    if (port < 2) {
        _spiPort = port;
    }
}

void LoRaClass::setSS(int ss) { _ss = ss; }
void LoRaClass::setReset(int reset) { _reset = reset; }
void LoRaClass::setDIO0(int dio0) { _dio0 = dio0; }

LoRaClass LoRa;
