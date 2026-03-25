#include <FlexCAN_T4.h>

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> CANbus;

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Starting CAN internal loopback test...");

  CANbus.begin();
  CANbus.setBaudRate(500000);
  CANbus.setLoopBack(true);
}

void loop() {
  CAN_message_t msg;
  msg.id = 0x321;
  msg.len = 2;
  msg.buf[0] = 0x12;
  msg.buf[1] = 0x34;
  CANbus.write(msg);
  Serial.println("Sent");

  CAN_message_t incoming;
  if (CANbus.read(incoming)) {
    Serial.print("Received: ID 0x");
    Serial.print(incoming.id, HEX);
    Serial.print(" Data: ");
    for (int i = 0; i < incoming.len; i++) {
      Serial.print(incoming.buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  delay(500);
}
