#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "wifi-ssid";
const char* password = "wifi-pass";
const char* api_host = "api.thingspeak.com";
const char* api_key = "api_key";  // API key

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);  
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi bağlanamadi tekrar deneniyor");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("WiFi'ya tekrar bağlandi");
  }

  client.setInsecure();  // SSL sertifikasını iptal ediyoruz 

  Serial.print("Things speak bağlaniyor.");
  
  if (!client.connect(api_host, 443)) {
    Serial.println("Connection failed");
    delay(5000);
    return;
  }
  
  Serial.println("Connected!");

  
  if (client.connect(api_host, 443)) {
    int sensorValue = analogRead(A0);  
    String postStr = "api_key=";
    postStr += api_key;
    postStr += "&field2=";
    postStr += String(sensorValue); 
    

    client.print("POST /update HTTP/1.1\nHost: api.thingspeak.com\nConnection: close\nContent-Type: application/x-www-form-urlencoded\nContent-Length: " + String(postStr.length()) + "\n\n" + postStr);

    Serial.println("data sent");
    client.stop();
    }else {
    Serial.println("connect failed");
    }
    
  delay(1000);  
}
