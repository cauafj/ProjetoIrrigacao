//OBS: ESSE CÓDIGO AINDA ESTAVA SENDO TRABALHADO, MAIS DETALHES NO README

#include <WiFi.h>
#include <esp_now.h>
#include "String.h"
#include <HTTPClient.h>

//variáveis de conexão
const char *ssid = "Paulo_2.4";
const char *pass = "ap672407";
char *server = "script.google.com";
char *GScriptId = "1n14hYrKd4es7MwOKs5sh0VLfxHEyFF_8xGqw6Q6YCLY";
int httpport = 443;

//estrutura para recebimento dos dados da ESP que controla o relé
typedef struct struct_message{
  float moisture;
  bool dripping;
  int waterEnd;
  int waterStart;
} struct_message;

struct_message planData;

//função para conectar wifi
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

//função para enviar os dados para a planilha
void sendInfo(float moisture, bool dripping, int stoppedDripping, int startedDripping) {
  HTTPClient http;
  //essa URL vem da planilha em si
  String url = String("https://script.google.com/macros/s/AKfycby0xDElrNz8tPjwY4GRxe68S1xOzq57zWz95mhiBh0Z13zWAcAw-vHp-ZuP7SyY_g/exec?") + "value1=" + moisture + "&value2=" + dripping + "&value3=" + stoppedDripping + "&value4=" + startedDripping;

  Serial.println("Sending data");
  http.begin(url.c_str());
  int httpCode = http.GET();
  //String payload;
  if (httpCode > 0) { //Check for the returning code
    //payload = http.getString();
    Serial.print("httpCode: ");
    Serial.println(httpCode);
    //Serial.println(payload);
  }
  else {
    Serial.print("Error on HTTP request: ");
    Serial.println(httpCode);
  }
  http.end();
}

//função disparada quando a ESP32 recebe os dados
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&planData, incomingData, sizeof(planData));

  Serial.println("umidade recebida");
  Serial.println(planData.moisture);

  Serial.print("dripping:");
  Serial.println(planData.dripping);
  
  //confere a conexão wifi, se não estiver funcionando, tenta conectar
  //if(WiFi.status() != WL_CONNECTED){
  //  connectWifi();
  //}

 //sendInfo(planData.moisture, planData.dripping, planData.waterEnd, planData.waterStart);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  //confere a conexão wifi, se não estiver funcionando, tenta conectar
  //if(WiFi.status() != WL_CONNECTED){
  //  connectWifi();
  //}
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("deule");
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  
}
