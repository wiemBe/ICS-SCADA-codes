#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "ssid";
const char* password = "passwd";
const char* api_host = "api.thingspeak.com";
const char* api_key = "";  // ThingSpeak API key
const char* api_host_weather = "api.openweathermap.org";
const char* api_key_weathermap = "";  // OpenWeatherMap API key
float derece;

WiFiClientSecure client;
WiFiClientSecure client2;

float parseJson(const String& jsonData){
  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, jsonData);

  if (error) {
    
    return 10000.0;
  }
  
  float temperature = doc["main"]["temp"];
  return temperature;
}


void setup() {
  Serial.begin(115200);

  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  client.setInsecure();
  client2.setInsecure();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("Reconnected to WiFi!");
  }
 
  // OpenWeatherMap API
  Serial.println("OpenWeatherMap API baglaniyor");
  if (client2.connect(api_host_weather, 443)) {
    client2.print("GET /data/2.5/weather?lat=40.93567&lon=29.15507&appid=" + 
              String(api_key_weathermap) +
              " HTTP/1.1\nHost: api.openweathermap.org\nConnection: close\n\n");

    Serial.println("OpenWeatherMap get istegi yollandi.");

    // Read response
    while (client2.connected() || client2.available()) {
      if (client2.available()) {
        String line = client2.readStringUntil('\n');
        
        derece=parseJson(line);
        if (derece != 10000.0){
          Serial.println(derece-273.15);
        }
      }    
    }
    
    client2.stop();
  } else {
    Serial.println("OpenWeatherMap baglanamadi.");
  }

  // ThingSpeak API
  Serial.println("ThingSpeak baglaniyor");
  if (client.connect(api_host, 443)) {
    float sensorValue = analogRead(A0);
    float dataToSent = sensorValue - (derece-273.15);  
    Serial.println(dataToSent);
    String postStr = "api_key=" + String(api_key) + "&field2=" + String(dataToSent);

    client.print("POST /update HTTP/1.1\nHost: api.thingspeak.com\nConnection: close\n"
                 "Content-Type: application/x-www-form-urlencoded\nContent-Length: " + 
                 String(postStr.length()) + "\n\n" + postStr);
    
    Serial.println("ThingSpeak veri yollandi");
    client.stop();
  } else {
    Serial.println("ThingSpeak baglanamadi");
  }

  delay(10000);  
}
