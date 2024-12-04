#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

WiFiClient wifiClient;
HTTPClient http;

const char* ssid = "ssid";
const char* password = "passw";

const int LED = 2;

const char* serverUrl = "http://192.168.54.219";

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    http.begin(wifiClient, serverUrl);
    int httpCode = http.GET();

    if (httpCode == 200) {
      digitalWrite(LED, HIGH);
    } else {
      Serial.printf("HTTP error code: %d\n", httpCode);
      digitalWrite(LED, LOW);
    }

    String response = http.getString();
    Serial.println("Response:");
    Serial.println(response);

    http.end();
  } else {
    Serial.println("Wi-Fi disconnected! Reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nReconnected to Wi-Fi");
  }

  delay(5000); 
}
