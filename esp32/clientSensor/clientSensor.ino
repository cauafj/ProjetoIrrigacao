#include <WiFi.h>
#include <esp_now.h>

int sensorPin = 14; //pino do sensor
float moisture; //variável de umidade
  
uint8_t receiverAddress[] = {0xE8, 0xDB,0x84, 0x0B, 0x58, 0x00}; //endereço da placa que recebe os dados

int id = 0; //determina um identificador para a placa

//definiçao da estrutura da mensagem para envio
typedef struct structMsg {
  float moisture;
  int id;
} structMsg;

structMsg mData; //variável que será usada para armazenar os dados

esp_now_peer_info_t peerInfo; //variável que irá guardar as informações do serverRele

//função disparda quando a ESP32 envia dados
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nStatus do último envio:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

//função que lê a umidade por determinada quantidade de segundos e faz a média aritmética dos valores encontrados
float getMoistureAvg(int seconds) {
  float sum = 0.0;
  float avg = 0.0;
  float result = 0.0;
  float valMap = 0.0;
  int ara0 = 0;
     
  for(int i = 0; i < seconds; i++) {
    ara0 = analogRead(sensorPin);  
    valMap = map(ara0, 4095, 0, 0 , 100);
    sum += valMap;
    delay(1000);
  } 
   
  result = sum/seconds;
  return result;
}

void setup() {
  Serial.begin(115200);

  pinMode(sensorPin, INPUT);

  WiFi.mode(WIFI_STA);

  //inicia o ESP-NOW ou informa qualquer erro ocorrido na tentativa
  if(esp_now_init() != 0) {
    Serial.println("Erro ao incializar o espnow");
    return;
  }

  //registra a função onDataSent que deve ser disparada quando houver envio de dados
  esp_now_register_send_cb(onDataSent);

  //copia o endereço do array para a estrutura
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0; //define o channel
  peerInfo.encrypt = false;

  //adiciona a conexão com o serverRele ou mostra o erro na tentativa
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("deule");
  }
}

void loop() {
  //lê a umidade por determinados segundos, 1 valor para cada segundo, (5 nesse caso) e retorna uma média da umidade nesse intervalo de temp
  moisture = getMoistureAvg(10);
  Serial.print("umidade em: ");
  Serial.println(moisture);

  mData.moisture = moisture;
  mData.id = id;

  //envia os dados para o serverRele
  esp_now_send(receiverAddress, (uint8_t *) &mData, sizeof(mData));
}
