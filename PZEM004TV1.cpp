#include "Arduino.h"
#include "PZEM004TV1.h"


PZEM004TV1::PZEM004TV1(HardwareSerial* port, uint8_t receivePin, uint8_t transmitPin)
{
    port->begin(9600, SERIAL_8N1, receivePin, transmitPin);
    _serial = port;
}

PZEM004TV1::~PZEM004TV1()
{
    delete _serial;
}

void PZEM004TV1::sendCommand(uint8_t command[], int length) {
    uint16_t crc = calculateCRC(command, length);
    // Serial.print("CRC: "); Serial.println(crc, HEX);
    // // Print the command in hex
    // Serial.print("Command Length"); Serial.println(sizeof(command));
    // Serial.print("Before Command: ");
    // for (int i = 0; i < length; i++) {
    //     Serial.print(command[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();
    uint8_t mCommand[length + 2];
    for (int i = 0; i < length; i++) {
        mCommand[i] = command[i];
    }
    mCommand[length] = crc & 0xFF;
    mCommand[length + 1] = crc >> 8;

    // //append crc to command
    // // Append crc to command
    // command.push(crc & 0xFF);
    // command.push(crc >> 8);

    //Print the command in hex
    // Serial.print("Command: ");
    // for (int i = 0; i < length +2 ; i++) {
    //     Serial.print(mCommand[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();

    while(_serial->available())
        _serial->read();

    _serial->write(mCommand, sizeof(mCommand));
    delay(150); // Wait for PZEM-004t to process the command
}

uint16_t PZEM004TV1::calculateCRC(uint8_t data[], int length) {
    uint16_t crc = 0xFFFF;

    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }

    return crc;
}

uint16_t PZEM004TV1::readRegisterValue() {
    uint8_t response[9];
    _serial->readBytes(response, 9);

    //print the response in hex
    // Serial.print("Response: ");
    // for (int i = 0; i < 9; i++) {
    //     Serial.print(response[i], HEX);
    //     Serial.print(" ");
    // }
    // Serial.println();


    //Check number of bytes received
    if (response[0] != 0x01 || response[0] == 0) {
        return 0;
    }
    else if (response[2] < 4) {
        return (response[3] << 8) | response[4];

    }else {
        uint16_t raw_low = (response[3] << 8) | response[4];
        uint16_t raw_high = (response[5] << 8) | response[6];

        return (raw_high << 16) | raw_low;
    }

}

void PZEM004TV1::readResponse(uint8_t response[], int length) {
    _serial->readBytes(response, length);
}

float PZEM004TV1::readVoltage() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x00, 0x00, 0x01};
    // Serial.print("Voltage CC length"); Serial.println(sizeof(command));
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 0.1;
}

float PZEM004TV1::readCurrent() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x01, 0x00, 0x02};
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 0.001;
}

float PZEM004TV1::readPower() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x03, 0x00, 0x02};
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 0.1;
}

float PZEM004TV1::readEnergy() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x05, 0x00, 0x02};
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 1.0;
}

float PZEM004TV1::readFrequency() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x07, 0x00, 0x01};
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 0.1;
}

float PZEM004TV1::readPowerFactor() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x08, 0x00, 0x01};
    sendCommand(command, sizeof(command));
    uint16_t rawValue = readRegisterValue();
    return rawValue * 0.01;
}

bool PZEM004TV1::checkAlarmStatus() {
    uint8_t command[] = {0x01, 0x04, 0x00, 0x09, 0x00, 0x01};
    sendCommand(command, sizeof(command));
    uint8_t response[7];
    readResponse(response, 7);
    return (response[3] == 0xFF && response[4] == 0xFF);
}

void PZEM004TV1::resetEnergy() {
    uint8_t command[] = {0x01, 0x42};
    sendCommand(command, sizeof(command));
    uint8_t response[5];
    readResponse(response, 5);
    //Print the response in hex
    Serial.print("Reset Energy Response: ");
    for (int i = 0; i < 5; i++) {
        Serial.print(response[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

}
