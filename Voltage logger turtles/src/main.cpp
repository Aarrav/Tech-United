#include <Arduino.h>

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
  Serial.begin(9600);
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

  // Step 2: Execute printing every 30ms
  if (currentMillis - lastPrintTime >= printInterval) {
    lastPrintTime = currentMillis;
    
    // Print the smoothed result
    Serial.println(smoothedVoltage, 2); 
  }
}