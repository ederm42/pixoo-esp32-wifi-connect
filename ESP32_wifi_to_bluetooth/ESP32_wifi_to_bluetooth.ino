#include <WiFi.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// --- Pixoo Bluetooth ---
uint8_t PIXOO_ADDR[6] = {0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78};  // AB:CD:12:34:56:78 (MAC Address)
const uint8_t PIXOO_CHANNEL = 2;

// --- Wi-Fi credentials ---
const char* WIFI_SSID = "YourWifiSSID";
const char* WIFI_PASS = "YourWifiPassword";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Start Bluetooth
  if(!SerialBT.begin("ESP32Client", true)) {
    Serial.println("Failed to start Bluetooth");
    while(1);
  }

  // Connect to Pixoo
  Serial.println("Connecting to Pixoo...");
  if(SerialBT.connect(PIXOO_ADDR, PIXOO_CHANNEL)) {
    Serial.println("✅ Connected to Pixoo!");
  } else {
    Serial.println("❌ Connection failed!");
  }

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to Wi-Fi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected: " + WiFi.localIP().toString());

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  WiFiClient client = server.available();
  if(client) {
    Serial.println("Client connected");
    // read incoming bytes
    std::vector<uint8_t> buffer;
    while(client.connected() && client.available()) {
      buffer.push_back(client.read());
    }

    // send to Pixoo
    if(SerialBT.connected()) {
      SerialBT.write(buffer.data(), buffer.size());
      Serial.printf("Sent %d bytes to Pixoo\n", buffer.size());
    }

    client.stop();
    Serial.println("Client disconnected");
  }
}
