
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char* ssid = "ssid";
const char* password = "passwd";

const int pwmPin = D5; 

ESP8266WebServer server(80);

// Motor control function
void setMotorSpeed(int speed) {
  analogWrite(pwmPin, speed); //  (0 - 1023)
  Serial.println("Motor hizi set: " + String(speed));
}


void landingPage() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Motor Kontrol</title>
    <style>
      body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
      #dial { margin: 20px auto; width: 200px; height: 200px; }
    </style>
  </head>
  <body>
    <h1>Motor Hiz Kontrolu</h1>
    <input type="range" id="dial" min="0" max="1023" value="0" />
    <p>Speed: <span id="speedValue">0</span></p>
    <script>
      const dial = document.getElementById("dial");
      const speedValue = document.getElementById("speedValue");

      // inputa göre hizi guncelle
      dial.addEventListener("input", () => {
        speedValue.innerText = dial.value;

        // hizi servera yolla
        fetch(`/motor?speed=${dial.value}`)
          .then(response => response.text())
          .then(data => console.log(data));
      });
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}


void handleMotorControl() {
  if (server.hasArg("speed")) {
    int speed = server.arg("speed").toInt(); 
    if (speed >= 0 && speed <= 1023) {
      setMotorSpeed(speed);
      server.send(200, "text/plain", "Motor hizi set: " + String(speed));
    } else {
      server.send(400, "text/plain", "Gecersiz hiz degeri (0 - 1023).");
    }
  } else {
    server.send(400, "text/plain", "Hiz parametresi eksik.");
  }
}

void setup() {
  
  pinMode(pwmPin, OUTPUT);
  setMotorSpeed(0); 

  Serial.begin(115200);

  Serial.printf("Connecting to %s", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");
  Serial.println(WiFi.localIP());

  server.on("/", landingPage);
  server.on("/motor", handleMotorControl);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
