#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ssid";
const char* password = "password";

ESP8266WebServer server(80);

void landingPage() {
  server.send(200, "text/plain", "GET request");
  server.send(404, "text/plain", "Somethings are broken");
  server.send(501, "text/plain", "i cant function");

}

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
  
  server.on("/",landingPage);

  server.begin();
  Serial.println("http server started");
}

void loop() {
  server.handleClient();
}