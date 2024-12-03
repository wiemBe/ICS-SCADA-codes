#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

WiFiClient wifiClient;

const char* ssid = "ssid";
const char* password = "pass";
const int LED = 2;

const char* serverIp = "192.168.1.120";


void setup() {
  Serial.begin(115200);
    
  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());  
  
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(wifiClient,serverIp);
    
    int httpCode = http.GET();
    
    if (httpCode == 200){
      digitalWrite(LED, HIGH);
    }

    Serial.printf("HTTP code:%d \n", httpCode);
    String response = http.getString();
    Serial.println("response:");
    Serial.println(response);

    http.end();
    
    }else{
      Serial.printf("get failed\n");
    }

  

}

void loop() {
  
}