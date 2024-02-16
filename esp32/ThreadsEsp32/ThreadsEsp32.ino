TaskHandle_t Task1;
TaskHandle_t Task2;

void Task1Testing(void * args) {
  for(int i = 0; i < 5; i++) {
     Serial.print("Task 1: ");
    Serial.println(i);
  }
}

void Task2Testing(void * args) {
  for(int i = 0; i < 10; i++) {
    Serial.print("Task 2: ");
    Serial.println(i);
  }
}


void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
                    Task1Testing,
                    "Task1",
                    10000,  
                    NULL,    
                    1,
                    &Task1,
                    0);  

  xTaskCreatePinnedToCore(
                    Task2Testing,
                    "Task2",
                    10000,  
                    NULL,    
                    1,
                    &Task2,
                    1);
}

void loop() {

}
