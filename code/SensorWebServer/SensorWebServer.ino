#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Adafruit_BME280.h>

const char* SSID = ".........";
const char* PASSWORD = "..........";

WebServer server(80);
Adafruit_BME280 sensor;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
}

void sendData() {
  String message = "{\n";
  message += "  \"temperature\": ";
  message += String(sensor.readTemperature(), 2);
  message += ",\n  \"humidity\": ";
  message += String(sensor.readHumidity(), 0);
  message += "\n}\n";
  server.send(200, "application/json", message);
}

void setup() {
  Serial.begin(115200);
  if (!sensor.begin(0x76)) {
    Serial.println("BME280-Sensor wurde nicht gefunden!");
    while (true)
      delay(10);
  }
  initWiFi();
  server.on("/data", sendData); 
  server.begin();
}

void loop() {
  server.handleClient();
  delay(2);
}
