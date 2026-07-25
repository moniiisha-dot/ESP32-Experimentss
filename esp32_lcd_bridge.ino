#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void showMessage(const String& line1, const String& line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1.substring(0, 16));
  lcd.setCursor(0, 1);
  lcd.print(line2.substring(0, 16));
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();
  showMessage("FaceTrack AI", "System Ready");
}

void loop() {
  if (!Serial.available()) {
    delay(10);
    return;
  }

  String command = Serial.readStringUntil('\n');
  command.trim();

  if (command == "WAIT") {
    showMessage("Waiting for", "a face...");
  } else if (command.startsWith("HELLO:")) {
    showMessage("Good morning", command.substring(6));
  } else if (command == "BLINK") {
    showMessage("Liveness Check", "Please blink");
  } else if (command == "LEFT") {
    showMessage("Liveness Check", "Turn left");
  } else if (command == "RIGHT") {
    showMessage("Liveness Check", "Turn right");
  } else if (command == "UP") {
    showMessage("Liveness Check", "Look up");
  } else if (command == "DOWN") {
    showMessage("Liveness Check", "Look down");
  } else if (command == "SMILE") {
    showMessage("Liveness Check", "Please smile");
  } else if (command.startsWith("VERIFIED:")) {
    showMessage("Verified", command.substring(9));
  } else if (command == "REGISTERED") {
    showMessage("Attendance", "Already marked");
  } else if (command == "FAILED") {
    showMessage("Verification", "Failed");
  } else {
    showMessage("Message", command);
  }
}
