//OBS: ESSE CÓDIGO AINDA ESTAVA SENDO TRABALHADO, MAIS DETALHES NO README

#include <WiFi.h>
#include <esp_now.h>

//variável utilizada para cronometrar os envios
int timeSinceLastSend = 0; 
int ESPlistSize = 3;
 
//definiçao da estrutura da mensagem para recebimento das umidades
typedef struct structMsg{
  float moisture;
  int id;
} structMsg;

structMsg mData; //variável para recebimento das umidades

//definiçao da estrutura da mensagem para envio dos dados da planilha
typedef struct struct_message {
  float moisture;
  bool dripping;
  int waterEnd;
  int waterStart;
} struct_message;

struct_message planData; //variável para envio de dados à planilha

float moistureList[ESPlistSize] = {};

float generalMoisture; //média de umidade entre os valores recebidos das ESP32 com sensores

bool dripping = 0;

int valvPin = 32;

const float DRY = 40; //limiar de umidade
const float SOAKED = 70; //limiar de umidade

esp_now_peer_info_t peerInfo; //variável que irá guardar as informações da placa responsável por enviar dados à planilha

uint8_t receiverAddress[] = {0xE8, 0x68, 0xE7, 0x26, 0xD3, 0xA8};//endereço da ESP32 que irá receber os dados 

//função disparada quando a ESP32 recebe dados
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&mData, incomingData, sizeof(mData));

  Serial.println("umidade recebida");
  Serial.println(mData.moisture);

  Serial.print("recebida de:");
  Serial.println(mData.id);

  moistureList[mData.id] = mData.moisture;

  Serial.println(moistureList[0]);
}

//função disparada quando a ESP32 envia dados
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus do último envio:\t");
  if(status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Delivery Success");
  } else {
    Serial.println(status);
  }
}

//função que calcula a média das umidades recebidas
float moistureAvg() {
  int len = ESPlistSize;
  float sum = 0;
  for(int i = 0; i < len; i++) {
    sum+=moistureList[i];
  }
  return sum/len;
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  pinMode(valvPin, OUTPUT);
  digitalWrite(valvPin, HIGH);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  } else {
    Serial.println("deule");
  }

  esp_now_register_send_cb(onDataSent);

  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("deule");
  }
  
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  delay(10000);
  //confere a conexão wifi, se não estiver funcionando, tenta conectar
  //if(WiFi.status() != WL_CONNECTED){
  //  connectWifi();
  //}
  
  generalMoisture = moistureAvg();

  //Serial.println(millis()/1000 - timeSinceLastSend);
  if(millis()/1000 - timeSinceLastSend >= 60){
    //sendInfo(moisture, dripping, 0, 0);

    planData.moisture = generalMoisture;
    planData.dripping = dripping;
    planData.waterEnd = 0;
    planData.waterStart = 0;
    
    //envia dados para a ESP32 da planilha
    esp_now_send(receiverAddress, (uint8_t *) &planData, sizeof(planData));
    
    timeSinceLastSend = millis()/1000;
  }

  //se a umidade for menor que o limiar, liga o relé, que começa a regar e envia dados de acordo para a ESP da planilha
  if(generalMoisture < DRY && not(dripping)) {
    digitalWrite(valvPin, LOW);//liga o relé
    Serial.print("motor ligado em umidade:");
    Serial.println(generalMoisture);
    Serial.println("----------------------------------------");
    dripping = 1;
    //sendInfo(generalMoisture , dripping, 0, 1);
    planData.moisture = generalMoisture;
    planData.dripping = dripping;
    planData.waterEnd = 0;
    planData.waterStart = 1;

    esp_now_send(receiverAddress, (uint8_t *) &planData, sizeof(planData));

  //se a umidade for maior que o limiar, liga o relé, que para de regar e envia dados de acordo para a ESP da planilha
  } else if (generalMoisture  > SOAKED && dripping) {
      digitalWrite(valvPin, HIGH);//desliga o relé
      Serial.print("motor desligado em umidade: ");
      Serial.println(generalMoisture);
      Serial.println("----------------------------------------");
      dripping = 0;    
      //sendInfo(generalMoisture , dripping, 1, 0);
      planData.moisture = generalMoisture;
      planData.dripping = dripping;
      planData.waterEnd = 1;
      planData.waterStart = 0;

      esp_now_send(receiverAddress, (uint8_t *) &planData, sizeof(planData));
  }
}
