const int sensorPin = A0; //entrada do sensor
const int valvPin = 8; //entradda do relé
const int dry = 25; //limiar de umidade
const int waterTime = 10; //tempo de rega
int moisture; //variável de umidade

void setup() {
  pinMode(valvPin, OUTPUT); 
  digitalWrite(valvPin, HIGH); //desliga o relé
  Serial.begin(9600);
}

void loop() {
  //lê a umidade e passa um valor a cada x segundos (i < x, agora está como 5)
  for(int i=0; i < 5; i++) {
    moisture = analogRead(sensorPin); //lê a umidade 
    moisture = map(moisture, 1023, 0, 0, 100); //mapeia os valores de 0 a 1023 para 0 a 100
    Serial.println(moisture); //printa o valor da umidade no monitor Serial
    delay(1000); //espera um segundo
  }
  //se a umidade for menor que o limiar, liga o relé, que começa a regar
  if(moisture < dry) {
    digitalWrite(valvPin, LOW);//liga o relé
    delay(waterTime*1000);//rega por determinado waterTime segundos
    digitalWrite(valvPin, HIGH);//desliga o relé
  }
}
