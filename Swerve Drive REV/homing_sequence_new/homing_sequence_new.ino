const int absEncPin = 19;
const int motorPWM = 37;

const int absEncPin2 = 28;
const int motorPWM2 = 1;

float Kp = 0.5;

int duty_cycle = 0;
float targetAngleCont = 0;

unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3; // 50ms

void setup() {
  Serial.begin(9600);
  pinMode(absEncPin, INPUT);
  pinMode(motorPWM, OUTPUT);

  pinMode(33, OUTPUT); //move1
  pinMode(0, OUTPUT);
  // Set the PWM frequency to 50 Hz (20ms period) on pin 33
  analogWriteFrequency(33, 50);
  analogWriteFrequency(0, 50);

  // Let's use a higher resolution for finer control, e.g., 12-bit (0-4095)
  analogWriteResolution(12);
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
    //Serial.print(highTime, 2);

    duty_cycle_local = (angle - targetAngle) * Kp;
    duty_cycle_local = constrain(duty_cycle_local, -35, 35);

    if (1 <= duty_cycle_local && duty_cycle_local <= 7)
      duty_cycle_local = 7;

    if (-7 <= duty_cycle_local && duty_cycle_local <= -1)
      duty_cycle_local = -7;

    Serial.print(" ");
    Serial.println(angle);

    int t_micro = 1500 + duty_cycle_local * 5;
    digitalWrite(motorPWM, HIGH);
    delayMicroseconds(t_micro);
    digitalWrite(motorPWM, LOW);
    delayMicroseconds(20000 - t_micro);
  }
}

void moveToAngle2(float targetAngle2)
{
  int duty_cycle_local = 1;
  unsigned long currTime = millis();
  while (millis() - currTime < 1000)
  {
    unsigned long highTime = pulseIn(absEncPin2, HIGH);
    unsigned long lowTime = pulseIn(absEncPin2, LOW);

    float angle = (highTime * 360.0) / (highTime + lowTime);
    angle = angle - 180;
    //Serial.print(highTime, 2);

    duty_cycle_local = (angle - targetAngle2) * Kp;
    duty_cycle_local = constrain(duty_cycle_local, -35, 35);

    if (1 <= duty_cycle_local && duty_cycle_local <= 7)
      duty_cycle_local = 7;

    if (-7 <= duty_cycle_local && duty_cycle_local <= -1)
      duty_cycle_local = -7;

    Serial.print(" ");
    Serial.println(angle);

    int t_micro = 1500 + duty_cycle_local * 5;
    digitalWrite(motorPWM2, HIGH);
    delayMicroseconds(t_micro);
    digitalWrite(motorPWM2, LOW);
    delayMicroseconds(20000 - t_micro);
  }
}



void loop() {

  int Dmove1 = 26;
  int Dmove2 = 8;

  // Example for the 1500us base + 25*5us = 1625us pulse:
  float pw_us_move1 = 1500.0 + (float)Dmove1 * 5.0;
  float pw_us_move2 = 1500.0 + (float)Dmove2 * 5.0;

  int pwm_move1 = (int)((pw_us_move1 / 20000.0) * 4095.0);
  int pwm_move2 = (int)((pw_us_move2 / 20000.0) * 4095.0);

  moveToAngle2(145);
  moveToAngle(145);
  long tijd = millis();
  while (millis() - tijd < 1000)
  {
    analogWrite(33, pwm_move1);
    analogWrite(0, pwm_move2);
  }
  analogWrite(33, (int)((1500 / 20000.0) * 4095.0));
  analogWrite(0, (int)((1500 / 20000.0) * 4095.0));


  moveToAngle2(55);
  moveToAngle(55);
  tijd = millis();
  while (millis() - tijd < 1000)
  {
    analogWrite(33, pwm_move1);
    analogWrite(0, pwm_move2);
  }
  analogWrite(33, (int)((1500 / 20000.0) * 4095.0));
  analogWrite(0, (int)((1500 / 20000.0) * 4095.0));

}
