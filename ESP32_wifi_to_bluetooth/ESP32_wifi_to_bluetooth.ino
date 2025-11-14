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
    Serial.println("SUCCESS: Connected to Pixoo!");
  } else {
    Serial.println("ERROR: Connection failed!");
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

// --- Send raw bytes to Pixoo over Bluetooth ---
size_t sendToPixoo(const std::vector<uint8_t> &data) {
    if (!SerialBT.connected()) return 0;
    return SerialBT.write(data.data(), data.size());
}

// --- Send HTTP response to client ---
void sendHttpResponse(WiFiClient &client, const String &status, const String &body) {
    client.printf("HTTP/1.1 %s\r\n", status.c_str());
    client.println("Content-Type: text/plain");
    client.printf("Content-Length: %d\r\n", body.length());
    client.println("Connection: close");
    client.println();
    client.print(body);
}

// --- Handle Pixoo request ---
void handlePixooRequest(WiFiClient &client) {
    Serial.println("Client connected");

    // Read incoming bytes into a vector
    std::vector<uint8_t> buffer;
    unsigned long last = millis();
    while (client.connected()) {
        while (client.available()) {
            buffer.push_back(client.read());
            last = millis();
        }
        if (millis() - last > 50) break; // simple timeout
        delay(1);
    }

    // Determine response
    String responseStatus;
    String responseBody;

    if (buffer.empty()) {
        responseStatus = "400 Bad Request";
        responseBody = "No data received";
    } else if (!SerialBT.connected()) {
        responseStatus = "503 Service Unavailable";
        responseBody = "Pixoo not connected via Bluetooth";
    } else {
        size_t sent = sendToPixoo(buffer);
        Serial.printf("Sent %d bytes to Pixoo\n", sent);

        responseStatus = "200 OK";
        responseBody = "Sent " + String(sent) + " bytes to Pixoo";
    }

    // Send HTTP response
    sendHttpResponse(client, responseStatus, responseBody);

    client.stop();
    Serial.println("Client disconnected");
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        handlePixooRequest(client);
    }
}
