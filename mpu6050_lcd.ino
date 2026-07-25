#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const uint8_t MPU_ADDR = 0x68;
const float TEMP_OFFSET = 20.0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int16_t readWord(uint8_t registerAddress) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(registerAddress);
  if (Wire.endTransmission(false) != 0) {
    return 0;
  }

  Wire.requestFrom(MPU_ADDR, static_cast<uint8_t>(2), static_cast<uint8_t>(true));
  if (Wire.available() < 2) {
    return 0;
  }

  return (Wire.read() << 8) | Wire.read();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

  lcd.init();
  lcd.backlight();
  lcd.print("MPU6050 Ready");
  delay(1500);
}

void loop() {
  int16_t rawTemperature = readWord(0x41);
  float temperatureC = rawTemperature / 340.0 + 36.53;
  float adjustedTemperature = temperatureC - TEMP_OFFSET;

  int16_t ax = readWord(0x3B);
  int16_t ay = readWord(0x3D);
  int16_t az = readWord(0x3F);

  Serial.printf(
    "Temp: %.2f C | AX: %d AY: %d AZ: %d\n",
    adjustedTemperature, ax, ay, az
  );

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(adjustedTemperature, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("AX:");
  lcd.print(ax / 1000);
  lcd.print(" AY:");
  lcd.print(ay / 1000);

  delay(1000);
}
