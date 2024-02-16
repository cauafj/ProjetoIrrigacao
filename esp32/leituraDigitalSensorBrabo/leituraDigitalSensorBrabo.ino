int sensorPin = 34;

void setup() {
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);
}

void loop() {
  //Serial.println(map(analogRead(sensorPin), 4095, 0, 0, 100));
  Serial.println(digitalRead(sensorPin));
  delay(1000);
}
