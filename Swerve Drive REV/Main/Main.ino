void BasicPWM_wheel(int pin, int duty_cycle) {
  Serial.print(pin);
  Serial.print("  ");
  Serial.println(duty_cycle);

  int t_micro = 1500 + duty_cycle*5;
  digitalWrite(pin, HIGH);
  delayMicroseconds(t_micro);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000-t_micro);
  
}

void setup(){
  pinMode(23, OUTPUT);
  //Serial.begin(115200);
}
  
void loop(){
  BasicPWM_wheel(23, 10);

}
