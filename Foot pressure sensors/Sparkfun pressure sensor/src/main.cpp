#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_MPL3115A2.h>

//Create an instance of the object
Adafruit_MPL3115A2 mpl;


void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!mpl.begin()) {
    Serial.println("MPL3115A2 not found");
    while (1);
  }

  mpl.setMode(MPL3115A2_BAROMETER);

  Serial.println("MPL3115A2 ready");
}



void loop() {
  float pressure = mpl.getPressure();   // Pa

  if (pressure > 2500)
    pressure = 0;

  Serial.println(pressure);

  //delay(50);
}
