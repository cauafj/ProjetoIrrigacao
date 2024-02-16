/*
 * autor: Cauã Jacques versão 1
 * autor: Roger Urdangarin versão 2
 * Setembro de 2022 - Osório Rio Grande do Sul
 *  
 */
#include <time.h>
#include <TimeLib.h>
#include <Wire.h>  

char timestamp[20]; // declare a character variable that contains time-stamp
const int sensorPin = A0; //entrada do sensor
const int valvPin = 8; //entradda do relé
const float DRY = 24; //limiar de umidade - seco
const float ENCHARCADO = 22.50; //limiar de umidade - molhado
const boolean TRUE = 1;
const boolean FALSE = 0;
float moisture; //variável de umidade
int cicloirrigacao = 0; //varíavel para contar os ciclos de irrigação
boolean dripping = FALSE; //booleano que armazena se o sistema está ou não realizando  irrigação

//função para gerenciar valores não inteiros
double myround(double x)
{
  double t;

  if (!isfinite(x))
      return(x);

  if (x >= 0.0) {
      t = floor(x);
      if (t - x <= -0.5)
          t += 1.0;
      return(t);
  } else {
          t = floor(-x);
          if (t + x <= -0.5)
              t += 1.0;
          return(-t);
  }      
}

//printa hora e data
void digitalClockDisplay()
{
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();  
}

//printa minutos e segundos com ":" antes, no caso de ser um tempo com apenas um alagarismo, printa um 0 antes dele
void printDigits(int digits)
{
  Serial.print(":"); //printa na tela ":"
  //se o dígito for menor que 10, printa um 0
  if(digits < 10)
    Serial.print('0'); //printa na tela um 0
  Serial.print(digits); //print os dígitos
}

//função que lê a umidade por determinados segundos, 1 valor para cada segundo, e retorna uma média da umidade nesse intervalo de tempo
float getMoistureAvg(int seconds) {
  float sum = 0.0;
  float avg = 0.0;
  float result = 0.0;
  float ValMap = 0.0;
  int ara0 = 0;
     
  for(int i = 0; i < seconds; i++) {
    ara0 = analogRead(sensorPin);  
    ValMap = map(ara0, 0, 1023, 0 , 30);
    //Serial.println(ara0, DEC);
    //Serial.println(ValMap, DEC);  
    sum = myround(sum + ValMap);
    delay(1000);
  } 
   
  result = sum/seconds;  

  Serial.print("umidade media em: ");
  Serial.println(result);
  return result;
}

void setup() {
  setTime(13,20,0,11,9,22);//define a hora manualmente
  Serial.begin(9600);
  Serial.println("desliga o relé control");
  pinMode(valvPin, OUTPUT);
  digitalWrite(valvPin, HIGH);  
}

void loop() {
  //lê a umidade por determinados segundos, 1 valor para cada segundo, (5 nesse caso) e retorna uma média da umidade nesse intervalo de temp
  Serial.print("Ciclos: ");
  Serial.println(cicloirrigacao);
  Serial.print("Dripping: ");
  Serial.println(dripping);
  Serial.println("----------------------------------------");
  moisture = getMoistureAvg(5);
  
  //se a umidade for maior que o limiar, liga o motor, a irrigação inicia
  if((moisture - DRY) > 0 && not(dripping)) {
    digitalClockDisplay();
    cicloirrigacao++;
    dripping = TRUE;
    Serial.print("umidade em: ");
    Serial.println(moisture);
    digitalWrite(valvPin, LOW);
  
  //se a umidade for menor que o limiar, desliga o motor, a irrigação para
  } else if ((moisture - ENCHARCADO) < 0 && dripping) {
      digitalClockDisplay();
      digitalWrite(valvPin, HIGH);
      Serial.print("motor desligado em humidade: ");
      Serial.println(moisture);
      dripping = FALSE;      
  }
}
