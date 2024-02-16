const int sensorPin = A0; //entrada do sensor
const int valvPin = 8; //entradda do relé
const int dry = 30; //limiar de umidade - seco
const int limit = 50; //limiar de umidade - úmido
int moisture; //variável de umidade

void setup() {
  pinMode(valvPin, OUTPUT); //define o pino do relé como OUTPUT
  digitalWrite(valvPin, HIGH); //desliga o relé
  Serial.begin(9600); //start Serial
}

void loop() {
  //lê a umidade e passa um valor a cada x segundos (i < x, agora está como 5)
  for(int i=0; i < 5; i++) {
    moisture = analogRead(sensorPin); //lê a umidade 
    moisture = map(moisture, 885, 730, 0, 100); //mapeia os valores de 0 a 1023 para 0 a 100
    Serial.println(moisture); //printa o valor da umidade no monitor Serial
    delay(1000); //espera um segundo
  }
  //se a umidade for menor que o limiar, liga o relé, que começa a regar
  if(moisture < dry) {
    digitalWrite(valvPin, LOW);//liga o relé
    
    //mantém regando até a umidade passar do limite
    while(moisture < limit){
      moisture = analogRead(sensorPin);
      moisture = map(moisture, 885, 730, 0, 100);
      Serial.println(moisture);
      delay(1000);
    }
    digitalWrite(valvPin, HIGH);//desliga o relé
  }
}
