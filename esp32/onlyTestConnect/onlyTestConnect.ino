#include "String.h"
#include <HTTPClient.h>
#include <WiFi.h>

const char *ssid = "Usguriconnect";
const char *pass = "jorgeeee";
char *server = "script.google.com";
char *GScriptId = "1n14hYrKd4es7MwOKs5sh0VLfxHEyFF_8xGqw6Q6YCLY";
int httpport = 443;

//função de conexão wifi
void connectWifi() {
  Serial.print("Connecting to: ");
  Serial.print(ssid);
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println();
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    connectWifi();
  }
}
