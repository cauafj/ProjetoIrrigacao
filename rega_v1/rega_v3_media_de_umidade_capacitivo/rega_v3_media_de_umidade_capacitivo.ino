const int sensorPin = A0; //entrada do sensor
const int sensorTPin = A1; //entrada do sensor 2
const int valvPin = 8; //entradda do relé
const int dry = 25; //limiar de umidade
const int waterTime = 10; //tempo de rega
int moisture1; //variável de umidade 1
int moisture2; //variavel de umidade 2
int moistureF; //média de umidade

void setup() {
  pinMode(valvPin, OUTPUT); 
  digitalWrite(valvPin, HIGH); //desliga o relé
  Serial.begin(9600);
}

void loop() {
  //lê a umidade e passa um valor a cada x segundos (i < x, agora está como 5)
  for(int i=0; i < 5; i++) {
    moisture1 = analogRead(sensorPin); //lê a umidade 1
    moisture2 = analogRead(sensorTPin); //lê a umidade 2
    moisture1 = map(moisture1, 1023, 0, 0, 100); //mapeia os valores de 0 a 1023 para 0 a 100 da umidade 1
    moisture2 = map(moisture2, 1023, 0, 0, 100); //mapeia os valores de 0 a 1023 para 0 a 100 da umidade 2
    moistureF = (moisture1 + moisture2) / 2; //calcula a média de umidade dos sensores
    Serial.println(moistureF); //printa o valor da umidade no monitor Serial
    delay(1000); //espera um segundo
  }
  //se a umidade for menor que o limiar, liga o relé, que começa a regar
  if(moistureF < dry) {
    digitalWrite(valvPin, LOW);//liga o relé
    delay(waterTime*1000);//rega por determinado waterTime segundos
    digitalWrite(valvPin, HIGH);//desliga o relé
  }
}
