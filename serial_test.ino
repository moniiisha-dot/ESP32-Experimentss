void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 serial communication started.");
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();

    if (message.length() > 0) {
      Serial.print("ESP32 received: ");
      Serial.println(message);
    }
  }

  static unsigned long previousMillis = 0;
  if (millis() - previousMillis >= 2000) {
    previousMillis = millis();
    Serial.println("ESP32 is running...");
  }
}
