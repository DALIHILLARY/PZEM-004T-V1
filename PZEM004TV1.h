#ifndef PZEM004TV1_h
#define PZEM004TV1_h

#include "Arduino.h"
#include <SoftwareSerial.h>

class PZEM004TV1 {
public:
    PZEM004TV1(uint8_t receivePin, uint8_t transmitPin);


    float readVoltage();
    float readCurrent();
    float readPower();
    float readEnergy();
    float readFrequency();
    float readPowerFactor();
    bool checkAlarmStatus();
    void resetEnergy();
    
    ~PZEM004TV1();


private:
    SoftwareSerial *_serial;

    void sendCommand(uint8_t command[], int length);
    uint16_t calculateCRC(uint8_t *data, int length);
    uint16_t readRegisterValue();
    void readResponse(uint8_t *response, int length);
};

#endif
