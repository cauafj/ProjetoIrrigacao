const int sensorPin = 34; //entrada do sensor
const int valvPin = 32; //entrada do relé 
int pa = 0;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(valvPin, OUTPUT);
  digitalWrite(valvPin, HIGH);
  Serial.begin(115200);
}

void loop() {
  Serial.print("umidade: ");
  Serial.println(map(analogRead(sensorPin), 4095, 0, 0 , 1000));
  
  if(pa == 0) { 
    digitalWrite(valvPin, LOW);
    pa = 1;
  }else {
    digitalWrite(valvPin, HIGH);
    pa = 0;
  }

  delay(1000);
}
