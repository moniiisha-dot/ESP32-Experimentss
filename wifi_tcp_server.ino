#include <WiFi.h>

const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

WiFiServer server(5000);

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting");
  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - start > 20000) {
      Serial.println("\nWi-Fi connection timed out.");
      return;
    }
  }

  Serial.println("\nConnected.");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();

  if (WiFi.status() == WL_CONNECTED) {
    server.begin();
    Serial.println("TCP server listening on port 5000.");
  }
}

void loop() {
  WiFiClient client = server.available();

  if (!client) {
    delay(10);
    return;
  }

  Serial.println("Client connected.");
  client.println("Hello from ESP32!");

  while (client.connected()) {
    if (client.available()) {
      String message = client.readStringUntil('\n');
      message.trim();

      Serial.print("Received: ");
      Serial.println(message);

      client.print("ESP32 received: ");
      client.println(message);
    }

    delay(10);
  }

  client.stop();
  Serial.println("Client disconnected.");
}
