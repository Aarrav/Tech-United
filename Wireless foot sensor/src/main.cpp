#include <Arduino.h>
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <ArduinoBLE.h>

//Create an instance of the object
MPL3115A2 myPressure;
BLEService pressureService("180C"); // custom UUID
BLEFloatCharacteristic pressureChar("2A56", BLERead | BLENotify);

void setup()
{
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(3); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

  if (!BLE.begin()) {
    Serial.println("BLE failed!");
    while (1);
  }

  BLE.setLocalName("PressureSensor");
  BLE.setAdvertisedService(pressureService);

  pressureService.addCharacteristic(pressureChar);
  BLE.addService(pressureService);

  BLE.advertise();
  Serial.println("BLE device ready");
}

float readPressure() {
  // Read pressure in hPa
  return myPressure.readPressure();
}

void loop()
{
  BLEDevice central = BLE.central();

  if (central) {
    while (central.connected()) {

      float pressure = readPressure(); // your function
      Serial.print("Pressure: ");
      Serial.println(pressure);
      pressureChar.writeValue(pressure);
      delay(25); // 40 Hz
    }
  }
}