#include <Encoder.h>
Encoder enc1(21, 22);
Encoder enc2(17, 18);
Encoder enc3(31, 32);
Encoder enc4(27, 26);

void setup() {
  // put your setup code here, to run once:

pinMode(19, INPUT);
/*
pinMode(18, INPUT);
pinMode(20, INPUT);
pinMode(21, INPUT);
*/
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  int out = digitalRead(19);
  Serial.println(out);
  /*
  int out1 = digitalRead(18);
  int out2 = digitalRead(20);
  int out3 = digitalRead(21);
  Serial.print(out);
  Serial.print(" ");
  Serial.print(out1);
  Serial.print(" ");
  Serial.print(out2);
  Serial.print(" ");
  Serial.println(out3);
  delay(2);
  */
/*
  int newPos1 = enc1.read();
  int newPos2 = enc2.read();
  int newPos3 = enc3.read();
  int newPos4 = enc4.read();

  Serial.print(newPos1*360/1300);
  Serial.print(" ");
  Serial.print(newPos2*360/1300);
  Serial.print(" ");
  Serial.print(newPos3*360/1300);
  Serial.print(" ");
  Serial.println(newPos4*360/1300);
*/
  delay(1);
  
}
