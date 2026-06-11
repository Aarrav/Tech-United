#include <Arduino.h>
#include <Wire.h>
#include "SparkFunMPL3115A2.h"


//Create an instance of the object
MPL3115A2 myPressure;

struct __attribute__((packed)) SensorData {
  uint32_t timestamp_ms;
  float pressure;
};


void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(115200);  // Start serial for output

  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(3); // Set Oversample to the recommended 128
  myPressure.enableEventFlags();

}

float readPressure() {
  return myPressure.readPressure();
}



void loop() {
  SensorData data;
  data.timestamp_ms = millis();
  data.pressure = readPressure();

  Serial.print("Time: ");
  Serial.print(data.timestamp_ms);
  Serial.print(" ms | Pressure: ");
  Serial.println(data.pressure);
  delay(100); // Delay for a second before the next reading
}
