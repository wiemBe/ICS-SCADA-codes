#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(8080);
const int led_Pin = D1;  

void setup() {
  Serial.begin(115200);
  pinMode(led_Pin, OUTPUT);
  digitalWrite(led_Pin, LOW); 
  
  Serial.print("Connecting to. ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New Client.");
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(led_Pin, HIGH);  
  } else if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(led_Pin, LOW);   
  }

  client.println("HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body><h1>ESP8266 LED Control</h1><p><a href=\"/LED=ON\">Turn On</a></p><p><a href=\"/LED=OFF\">Turn Off</a></p></body></html>");
  delay(1);
  Serial.println("Client disconnected.");
}