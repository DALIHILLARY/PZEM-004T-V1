# PZEM004TV1 Library

**Author:** Dali Hillary  
**Role:** Software & Hardware Engineer

## Overview

The PZEM004TV1 library is designed to interface with the PZEM-004T V1.0 module, a power monitoring device. This library provides an easy-to-use set of functions to read various electrical parameters such as voltage, current, power, energy, frequency, power factor, and alarm status.

## Features

- **Simple Interface:** Easy-to-use functions for reading electrical parameters.
- **SoftwareSerial Support:** Utilizes the SoftwareSerial library for communication, allowing flexible pin assignment.
- **CRC Calculation:** Includes a CRC calculation method to ensure data integrity during communication.

## Installation

1. Download the library files from the repository.
2. Extract the files into your Arduino libraries folder.
3. Rename the extracted folder to "PZEM004TV1" if necessary.

## Usage

1. **Include the Library:**
    ```cpp
    #include "PZEM004TV1.h"
    ```

2. **Instantiate the PZEM004TV1 Object:**
    ```cpp
    PZEM004TV1 pzem(RX_PIN, TX_PIN); 
    ```

3. **Read Electrical Parameters:**
    ```cpp
    float voltage = pzem.readVoltage();
    float current = pzem.readCurrent();
    float power = pzem.readPower();
    float energy = pzem.readEnergy();
    float frequency = pzem.readFrequency();
    float powerFactor = pzem.readPowerFactor();
    bool alarmStatus = pzem.checkAlarmStatus();
    ```

4. **Reset Energy:**
    ```cpp
    pzem.resetEnergy();
    ```

## Notes

- Ensure proper connections of the PZEM-004T V1.0 module to the Arduino.
- Refer to the datasheet for the PZEM-004T module for specific details on electrical parameters and communication.

## Example

```cpp
#include "PZEM004TV1.h"

PZEM004TV1 pzem(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {
  float voltage = pzem.readVoltage();
  float current = pzem.readCurrent();
  float power = pzem.readPower();
  float energy = pzem.readEnergy();
  float frequency = pzem.readFrequency();
  float powerFactor = pzem.readPowerFactor();
  bool alarmStatus = pzem.checkAlarmStatus();

  Serial.println("Voltage: " + String(voltage) + " V");
  Serial.println("Current: " + String(current) + " A");
  Serial.println("Power: " + String(power) + " W");
  Serial.println("Energy: " + String(energy) + " kWh");
  Serial.println("Frequency: " + String(frequency) + " Hz");
  Serial.println("Power Factor: " + String(powerFactor));
  Serial.println("Alarm Status: " + String(alarmStatus));

  delay(5000);
}
