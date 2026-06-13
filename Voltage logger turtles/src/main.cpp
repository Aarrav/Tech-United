#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi credentials
const char* WIFI_SSID  = "Tech_United";       
const char* WIFI_PASS  = "RoboCupMSL";

// UDP settings
const char* laptopIP = "10.0.0.238";  // Your laptop's IP
const int udpPort = 5005;
WiFiUDP udp;

// Timing variables
unsigned long lastSampleTime = 0;
unsigned long lastPrintTime = 0;
const unsigned long sampleInterval = 10; // Sample every 10ms
const unsigned long printInterval = 30;  // Print every 30ms

// Filter variables
float smoothedVoltage = 0.0;
const float alpha = 0.2; // Smoothing factor: 0.0 to 1.0 (Lower = smoother)
bool firstSample = true; // Flag to initialize the filter on the first pass

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi
  Serial.println("\n\nStarting WiFi connection...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // Step 1: Execute sampling and filtering every 10ms
  if (currentMillis - lastSampleTime >= sampleInterval) {
    lastSampleTime = currentMillis;

    // Read and calculate the raw real voltage
    int rawData = analogRead(A1);
    float measuredVoltage = rawData * (3.3f / 4095.0f) * ((1000.0f + 180.0f) / 180.0f);
    float realVoltage = (-0.0146f * measuredVoltage * measuredVoltage) + (1.290f * measuredVoltage) - 0.297f;

    // Apply the Exponential Moving Average filter
    if (firstSample) {
      smoothedVoltage = realVoltage; // Seed the filter to avoid starting from zero
      firstSample = false;
    } else {
      smoothedVoltage = (alpha * realVoltage) + ((1.0f - alpha) * smoothedVoltage);
    }
  }

  // Step 2: Send via UDP every 30ms
  if (currentMillis - lastPrintTime >= printInterval) {
    lastPrintTime = currentMillis;

    if (WiFi.status() == WL_CONNECTED) {
      char buffer[64];
      sprintf(buffer, "%lu,%.2f", currentMillis, smoothedVoltage);

      udp.beginPacket(laptopIP, udpPort);
      udp.write((uint8_t*)buffer, strlen(buffer));
      udp.endPacket();
    }

    // Also print to serial for debugging
    Serial.print(currentMillis);
    Serial.print(" ms, ");
    Serial.println(smoothedVoltage, 2);
  }
}