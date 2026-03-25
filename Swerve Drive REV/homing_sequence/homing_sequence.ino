const int absEncPin = 19;
const int motorPWM = 37;

float Kp = 0.5;

int duty_cycle = 0;
float targetAngleCont = 0;

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3; // 50ms

void setup() {
  Serial.begin(9600);
  pinMode(absEncPin, INPUT);
  pinMode(motorPWM, OUTPUT);
}

void moveToAngle(float targetAngle)
{
  int duty_cycle_local = 1;
  unsigned long currTime = millis();
  while (millis() - currTime < 1000)
  {
    unsigned long highTime = pulseIn(absEncPin, HIGH);
    unsigned long lowTime = pulseIn(absEncPin, LOW);

    float angle = (highTime * 360.0) / (highTime + lowTime);
    angle = angle - 180;
    Serial.print(highTime, 2);

    duty_cycle_local = (angle - targetAngle) * Kp;
    duty_cycle_local = constrain(duty_cycle_local, -35, 35);

    if (1 <= duty_cycle_local && duty_cycle_local <= 7)
      duty_cycle_local = 7;

    if (-7 <= duty_cycle_local && duty_cycle_local <= -1)
      duty_cycle_local = -7;

    Serial.print(" ");
    Serial.println(duty_cycle_local);

    int t_micro = 1500 + duty_cycle_local * 5;
    digitalWrite(motorPWM, HIGH);
    delayMicroseconds(t_micro);
    digitalWrite(motorPWM, LOW);
    delayMicroseconds(20000 - t_micro);
  }
}

void moveToAngleContinuous(float targetAngleCont)
{
  unsigned long highTime = pulseIn(absEncPin, HIGH);
  unsigned long lowTime = pulseIn(absEncPin, LOW);

  float angle = (highTime * 360.0) / (highTime + lowTime);
  angle = angle - 180;
  Serial.println(angle, 2);

  duty_cycle = (angle - targetAngleCont) * Kp;
  duty_cycle = constrain(duty_cycle, -35, 35);

  if (1 <= duty_cycle && duty_cycle <= 7)
    duty_cycle = 7;

  if (-7 <= duty_cycle && duty_cycle <= -1)
    duty_cycle = -7;

  //Serial.print(" ");
  //Serial.println(duty_cycle);
}

void loop() {
  /*
    int pot = analogRead(21);
    targetAngleCont = map(pot, 600, 700, 0, 360) - 180;
    moveToAngleContinuous(targetAngleCont);
    Serial.print(" ");
    Serial.print(targetAngleCont);
    Serial.print("  ");
    



  // Keep sending PWM signal every 20ms
  int t_micro = 1500 + duty_cycle * 5;
  digitalWrite(motorPWM, HIGH);
  delayMicroseconds(t_micro);
  digitalWrite(motorPWM, LOW);
  delayMicroseconds(20000 - t_micro);
  */
  moveToAngle(180);
}
