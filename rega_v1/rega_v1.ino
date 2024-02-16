const int sensorPin = A0; //pino do sensor
const int valvPin = 8; //pino do relé
const int dry = 30; //limiar de umidade
const int waterTime = 10; //tempo de irrigação
int moisture; //variável de umidade

void setup() {
  pinMode(valvPin, OUTPUT); //definindo o pino do relé como de saída
  digitalWrite(valvPin, HIGH); //desliga o relé
  Serial.begin(9600); //define a taxa de transferência para comunicação serial
}

void loop() {
  //lê a umidade e passa um valor a cada x segundos (i < x, agora está como 5)
  for(int i=0; i < 5; i++) {
    moisture = analogRead(sensorPin); //lê a umidade 
    moisture = map(moisture, 1023, 0, 0, 100); //mapeia os valores de 0 a 1023 para 0 a 100
    Serial.println(moisture); //printa o valor da umidade no monitor serial
    delay(1000); //espera um segundo
  }
  //se a umidade for menor que o limiar, liga o relé, que começa a regar
  if(moisture < dry) {
    digitalWrite(valvPin, LOW);//liga o relé
    delay(waterTime*1000);//rega pela quantidade de segundos determinada na variável waterTime
    digitalWrite(valvPin, HIGH);//desliga o relé
  }
}
