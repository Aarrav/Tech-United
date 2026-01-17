/****************************************************************************************************************************
  PWM_Basic.ino

  For Teensy boards (Teensy 2.x, Teensy LC, Teensy 3.x, Teensy 4.x, etc.)
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/Teensy_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

#define _PWM_LOGLEVEL_       4

#include "Teensy_PWM.h"

#define USING_FLEX_TIMERS      true

#if USING_FLEX_TIMERS
  #define wheel_pin       23
  #define rotate_pin       14
#endif

//creates pwm instance
Teensy_PWM* Wheel;
Teensy_PWM* Rotate;

float frequency = 1000.0f;

float dutyCycle = 0.0f;

void setup()
{
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  delay(500);

#if USING_FLEX_TIMERS
  Serial.print(F("\nStarting PWM_Basic using FlexTimers on "));
#else
  Serial.print(F("\nStarting PWM_Basic using QuadTimers on "));
#endif

  Serial.println(BOARD_NAME);
  Serial.println(TEENSY_PWM_VERSION);

  //assigns PWM frequency of 1.0 KHz and a duty cycle of 0%
  Wheel = new Teensy_PWM(wheel_pin, frequency, dutyCycle);
  Rotate = new Teensy_PWM(rotate_pin, frequency, dutyCycle);

  if ( (!Wheel) || !Wheel->isPWMEnabled())
  {
    Serial.print(F("Stop here forever"));

    while (true)
      delay(10000);
  }
}

void loop()
{
  // You can change frequency here, anytime
  frequency = 100.0f;
  float i = 0.00;
  float sin_out = 0.00;
  float cos_out = 0.00;

  for (i; i<100000.0; i=i+1.0)
  {
    sin_out = sin(i/100);
    dutyCycle = 15.0 + sin_out;
    //Serial.println(dutyCycle);
    Wheel->setPWM(wheel_pin, frequency, dutyCycle);

    cos_out = cos(i/100);
    dutyCycle = 15.0 + cos_out;
    Rotate->setPWM(rotate_pin, frequency, dutyCycle);
    delay(10);
  }
  
/*
  dutyCycle = 15.5f;
  Wheel->setPWM(wheel_pin, frequency, dutyCycle);
  dutyCycle = 15.0f;
  Rotate->setPWM(rotate_pin, frequency, dutyCycle);
  delay(1000);

  dutyCycle = 15.0f;
  Wheel->setPWM(wheel_pin, frequency, dutyCycle);
  dutyCycle = 15.5f;
  Rotate->setPWM(rotate_pin, frequency, dutyCycle);
  delay(1000);

  dutyCycle = 15.5f;
  Wheel->setPWM(wheel_pin, frequency, dutyCycle);
  dutyCycle = 15.5f;
  Rotate->setPWM(rotate_pin, frequency, dutyCycle);
  delay(1000);

  dutyCycle = 15.0f;
  Wheel->setPWM(wheel_pin, frequency, dutyCycle);
  dutyCycle = 15.0f;
  Rotate->setPWM(rotate_pin, frequency, dutyCycle);
  delay(1000);
*/
}