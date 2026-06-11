#include <Arduino.h>
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <WiFi.h>
#include <WiFiUdp.h>

const char* WIFI_SSID  = "Tech_United";       
const char* WIFI_PASS  = "RoboCupMSL";
const char* LAPTOP_IP  = "10.0.0.30";
const int   REAL_UDP_PORT = 5005;
const int   FAKE_UDP_PORT = 5006;
const int   SEND_HZ    = 50;

MPL3115A2 myPressure;
WiFiUDP realUdp;
WiFiUDP fakeUdp;

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
  
  realUdp.begin(REAL_UDP_PORT);
  fakeUdp.begin(FAKE_UDP_PORT);

  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  
  myPressure.setOversampleRate(3); // Set Oversample to the recommended 128
  myPressure.enableEventFlags();
}

float readPressure() {
  return myPressure.readPressure();
}

float readFakePressure(uint32_t timestamp_ms, float realPressure) {
  const float wave = sin(timestamp_ms * 0.002f) * 30.0f;
  const float slowDrift = sin(timestamp_ms * 0.0002f) * 80.0f;

  return realPressure + 500.0f + wave + slowDrift;
}

void sendSensorData(WiFiUDP& udp, uint16_t port, const SensorData& data) {
  udp.beginPacket(LAPTOP_IP, port);
  udp.write((const uint8_t*)&data, sizeof(data));
  udp.endPacket();
}

void loop() {
  const uint32_t timestamp_ms = millis();
  const float realPressure = readPressure();

  SensorData realData;
  realData.timestamp_ms = timestamp_ms;
  realData.pressure = realPressure;

  SensorData fakeData;
  fakeData.timestamp_ms = timestamp_ms;
  fakeData.pressure = readFakePressure(timestamp_ms, realPressure);

  Serial.print("Time: ");
  Serial.print(timestamp_ms);
  Serial.print(" ms | Real pressure: ");
  Serial.print(realData.pressure);
  Serial.print(" Pa | Fake pressure: ");
  Serial.println(fakeData.pressure);

  sendSensorData(realUdp, REAL_UDP_PORT, realData);
  sendSensorData(fakeUdp, FAKE_UDP_PORT, fakeData);

  // Enforce the SEND_HZ rate.
  delay(1000 / SEND_HZ);                    
}
