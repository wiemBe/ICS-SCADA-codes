#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "wifi_ssid";
const char* password = "wifi_pass";
const char* api_host = "api.openai.com";
const char* openai_key = "your_openai_key";  //  OpenAI API key

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

  Serial.print("OPEN AI bağlaniyor.");
  
  if (!client.connect(api_host, 443)) {
    Serial.println("Connection failed");
    delay(5000);
    return;
  }
  
  Serial.println("Connected!");

  // JSON payload yaratıyorz
  StaticJsonDocument<500> doc;  
  doc["model"] = "gpt-3.5-turbo";
  JsonArray messages = doc.createNestedArray("messages");
  JsonObject message = messages.createNestedObject();
  message["role"] = "user";
  message["content"] = "Hello!";

  String json;
  serializeJson(doc, json);

  // HTTP POST atıyoruz
  String request = String("POST /v1/chat/completions HTTP/1.1\r\n") +
                  "Host: " + api_host + "\r\n" +
                  "Authorization: Bearer " + openai_key + "\r\n" +
                  "Content-Type: application/json\r\n" +
                  "Content-Length: " + json.length() + "\r\n" +
                  "Connection: close\r\n\r\n" +
                  json;

  
  client.print(request);
  
  // response bekliyoruz
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }

  
  String response = client.readString();
  Serial.println("Response:");
  Serial.println(response);

  // json parslıyoruz
  StaticJsonDocument<1024> responseDoc;
  DeserializationError error = deserializeJson(responseDoc, response);

  if (error) {
    Serial.print("Failed to parse response: ");
    Serial.println(error.c_str());
  } else {
    const char* content = responseDoc["choices"][0]["message"]["content"];
    Serial.println("Assistant Response:");
    Serial.println(content);
  }

  client.stop();
  delay(10000);  
}
