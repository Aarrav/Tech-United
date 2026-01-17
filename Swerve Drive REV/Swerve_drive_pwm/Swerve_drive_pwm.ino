#include <Servo.h>

Servo myServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(33, OUTPUT);

  //myServo.attach(33);
}

void loop() {
  // put your main code here, to run repeatedly:
    int duty_cycle = 25;
    int t_micro = 1500 + duty_cycle*5;
    digitalWrite(33, HIGH);
    delayMicroseconds(t_micro);
    digitalWrite(33, LOW);
    delayMicroseconds(20000-t_micro);
    Serial.println("done");
  
}

//0 1
//37 33
  