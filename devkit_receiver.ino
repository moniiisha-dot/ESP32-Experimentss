#include <Arduino.h>

HardwareSerial CamSerial(2);

const int RX_PIN = 16;
const int TX_PIN = 17;
const uint32_t MAX_IMAGE_SIZE = 200000;

bool readExact(Stream& stream, uint8_t* buffer, size_t length, unsigned long timeoutMs) {
  size_t received = 0;
  unsigned long start = millis();

  while (received < length) {
    if (stream.available()) {
      buffer[received++] = stream.read();
      start = millis();
    } else if (millis() - start > timeoutMs) {
      return false;
    }
  }

  return true;
}

void setup() {
  Serial.begin(115200);
  CamSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("Waiting for ESP32-CAM image...");
}

void loop() {
  if (CamSerial.available() < 4) {
    delay(10);
    return;
  }

  uint32_t imageSize = 0;

  if (!readExact(
        CamSerial,
        reinterpret_cast<uint8_t*>(&imageSize),
        sizeof(imageSize),
        2000
      )) {
    Serial.println("Could not read image length.");
    return;
  }

  if (imageSize == 0 || imageSize > MAX_IMAGE_SIZE) {
    Serial.printf("Invalid image size: %u\n", imageSize);
    return;
  }

  Serial.printf("Receiving %u bytes...\n", imageSize);

  uint8_t buffer[256];
  uint32_t remaining = imageSize;

  while (remaining > 0) {
    size_t chunk = remaining > sizeof(buffer) ? sizeof(buffer) : remaining;

    if (!readExact(CamSerial, buffer, chunk, 3000)) {
      Serial.println("Image transfer timed out.");
      return;
    }

    // Forward raw JPEG bytes to the computer through USB serial.
    Serial.write(buffer, chunk);
    remaining -= chunk;
  }

  Serial.flush();
}
