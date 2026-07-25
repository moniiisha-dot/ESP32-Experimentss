#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled for this ESP32 board.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Bridge");
  Serial.println("Bluetooth device started as ESP32_Bridge.");
}

void loop() {
  while (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  while (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  delay(10);
}
