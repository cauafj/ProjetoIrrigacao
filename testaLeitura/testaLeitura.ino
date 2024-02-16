const int sensorPin = 8; //entrada do sensor
const int valvPin = 7; //entrada do relé
int moisture; //variável de umidade
int totalSeg; 
int varSeg;
int readTime = 60; //define o tempo de leitura em segundos

int variation[60][2]; //matriz que armazena as leituras coluna1: valor lido, coluna2: quantidade de vezes que foi lido

int line = 0; //variável que armazena a última linha adicionada na matriz
//int column = 0;

//veirifica se um número já está presente na primeira coluna da matriz
boolean verifyExistance(int num)
{
  for(int i = 0; i < readTime; i++) 
  {
    if(variation[i][0] == num)
    {
      return true;
    }
  }
  return false;
}

//verifica em qual linha da matriz o número está localizado
int lineOf(int num)
{
  for(int i = 0; i < readTime; i++)
  {
   if(num == variation[i][0])
   {
     return i;
   }
  }
}

//preenche uma coluna com um valor determinado
void fillLine(int num, int line)
{
  for(int i = 0; i < readTime; i++)
  {
   variation[i][line] = num;
  }
}

//calcula a média dos valores da matriz
float calcAvg()
{
  float sum = 0.0;
  float avg = 0.0;
  for(int i = 0; i < readTime; i++)
  {
   sum+=variation[i][0]*variation[i][1];
  }
  avg = sum/readTime;
  return avg;
}

//printa todos os valores da matriz seguidos pela média
void printValues()
{
  for(int i = 0; i < readTime; i++)
  {
   Serial.print(variation[i][0]);
   Serial.print(" ");
   Serial.println(variation[i][1]);
  }
  
  Serial.print("MEDIA: ");
  Serial.println(calcAvg());
  Serial.println("-------------------------------");
}

void setup() {
  pinMode(valvPin, OUTPUT);
  digitalWrite(valvPin, HIGH);
  Serial.begin(9600);

  fillLine(1, 1);
  fillLine(0, 0);
}

void loop() {
  for(int i = 0; i < readTime; i++)
  {
    moisture = digitalRead(sensorPin);
    if(!verifyExistance(moisture))
    {
      variation[line][0] = moisture;
      line++;
    
    } else {
      variation[lineOf(moisture)][1]+=1;
    }
    delay(1000);
  }
  printValues();
  fillLine(1, 1);
  fillLine(0, 0);
  line = 0;
}
