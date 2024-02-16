int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
}

void loop() {
  Serial.print("umidade:");
  Serial.println(map(analogRead(sensorPin), 1023, 0, 0 , 500));

  delay(1000);
}
