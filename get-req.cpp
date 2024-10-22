#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "senin wifi ismin";
const char* password = "wifi şifren";
const char* host = "172.217.16.206";  // google.com'un ip adresi

WiFiClient client;

void setup() {
  pinMode(D4, OUTPUT);
  Serial.begin(115200);  

  // Wi-Fi'ya bağlanıyor
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // led her turda yanıp sönücek
  digitalWrite(D4, HIGH);
  delay(5000);
  digitalWrite(D4, LOW);
  delay(5000);

  // google.com a http req atılıyor
  Serial.print("Connecting to ");
  Serial.println(host);

  if (client.connect(host, 80)) {  // google bağlanıyor port 80'den
    Serial.println("Connection successful!");

    // HTTP requesti yolluyoruz
    client.print(String("GET / HTTP/1.1\r\n") +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    delay(500);  

    // Gelen veriyi oku
    while (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);  // serial porttan datayı ekrana yazdırıyor
    }

    client.stop();  
    Serial.println("Disconnected.");
  } else {
    Serial.println("Connection failed.");
  }
  
  delay(10000);  
}
