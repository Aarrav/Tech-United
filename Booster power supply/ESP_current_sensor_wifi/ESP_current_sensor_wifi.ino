#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Aarrav5g";
const char* password = "aarrav0217";

WebServer server(80);

float offset = 2880.0;
const int numSamples = 10;
float current = 0.0;

unsigned long lastSampleTime = 0;
const unsigned long sampleIntervalMs = 50; // 20 Hz

void handleRoot() {
  server.send(200, "text/plain", String(current, 3));
}

void setup() {
  Serial.begin(115200);

  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Web server route
  server.on("/", handleRoot);
  server.begin();
}

void loop() {

  server.handleClient();  // ALWAYS first

  unsigned long now = millis();
  if (now - lastSampleTime >= sampleIntervalMs) {
    lastSampleTime = now;

    float sum = 0.0;
    for (int i = 0; i < numSamples; i++) {
      sum += analogRead(A2);
    }

    float avgRawVal = sum / numSamples;
    float rawCurrent = (avgRawVal - offset) / 66.0;
    current = rawCurrent * 0.699 + 0.825;

    Serial.println(current);
}
}
/*

float offset = 2880.0;
const int numSamples = 10;

void setup() {
  Serial.begin(115200);
}

void loop() {

  float sum = 0.0;

  // Take 10 samples
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(A4);
    delay(5);  // small delay between samples
  }

  // Calculate average raw value
  float avgRawVal = sum / numSamples;

  // Convert to current
  float rawCurrent = (avgRawVal - offset) / 82.5;
  float current = rawCurrent * 0.866 + 0.15;

  // Print averaged current
  Serial.println(current);
}
*/
