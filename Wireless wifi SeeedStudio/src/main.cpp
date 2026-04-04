#include <Arduino.h>
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <WiFi.h>
#include <WiFiUdp.h>

const char* WIFI_SSID  = "Tech_United";       
const char* WIFI_PASS  = "RoboCupMSL";
const char* LAPTOP_IP  = "10.0.0.238";
const int   UDP_PORT   = 5005;
const int   SEND_HZ    = 50;

MPL3115A2 myPressure;
WiFiUDP udp;

// Create a structure to pack the timestamp and pressure together
struct __attribute__((packed)) SensorData {
  uint32_t timestamp_ms;
  float pressure;
};

void setup() {
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  
  udp.begin(UDP_PORT);

  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(3); // Set Oversample to the recommended 128
  myPressure.enableEventFlags();
}

float readPressure() {
  return myPressure.readPressure();
}

void loop() {
  // Populate the struct with the current time and pressure
  SensorData data;
  data.timestamp_ms = millis();
  data.pressure = readPressure();

  Serial.print("Time: ");
  Serial.print(data.timestamp_ms);
  Serial.print(" ms | Pressure: ");
  Serial.println(data.pressure);

  // Send the entire struct over UDP
  udp.beginPacket(LAPTOP_IP, UDP_PORT);
  udp.write((uint8_t*)&data, sizeof(data));
  udp.endPacket();

  // Enforce the SEND_HZ rate (20Hz = 50ms per loop)
  delay(1000 / SEND_HZ);                    
}