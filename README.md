# Projeto de pesquisa - Sistema de irrigação autônomo, inteligente e de baixo custo

Abaixo estão explicações sobre a finalidade e utilizaçõ de cada um dos códigos presentes nos arquivos do presente repositório. As explicações no README serão breves, pois cada arquivo conta com comentários que visam possibilitar o entedimento da lógica.
Além disso, há a pasta [_noCode_](./noCode), a qual conta com arquivos que não são de código.

**OBS:** todos os códigos que estão fora da pasta [esp32](./esp32/) foram inicialmente desenvolvidos para a utilização com Arduino, o contrário é válido para os arquivos dentro da pasta. Contudo, muitos dos códigos podem ser (e vários foram) utilizados nas duas placas, somente são necessárias as mudanças:

| Arduino  | ESP32 |
| ------------- | ------------- |
| Serial.begin(9600)  | Serial.begin(115200)  |
| map(leitura, 1023, 0, 0, 100),   | map(leitura, 4095, 0, 0, 100)  | 


## codigoroger

Esta pasta contém duas o _codigoroger.ino_ e o _codigorogerv2.ino_. O primeiro foi feito pelo professor [Roger Gonçalves Urdangarin](https://github.com/rurdangarin), atual orientador do projeto, com adições do então bolsista. O código tinha como objetivo aperfeiçoar a lógica utilizada no sistema, principalmente com a adição da função _getMoistureAvg_, a qual realiza leituras por segundo durante um determinado tempo e, após, faz a média dos valores, para assim se aproximar mais do valor real e diminuir problemas com oscilações nas leituras.
O segundo código é o primeiro passo em direção ao futuro do projeto, com as primeiras modificações feitas ao seu predecessor, por exemplo, comentando a função _myround_, a qual se provou desnecessária.

## esp32

Esta pasta conta com diversos códigos, os quais serão explicados abaixo, todos criados a partir do momento que se decidiu utilizar a ESP32 no lugar do Arduino por conta de custo-benefício, conectivdade e desempenho.

* **clientSensor:** Feito quando se decidiu utilizar o protocolo [ESP-NOW](https://www.espressif.com/en/solutions/low-power-solutions/esp-now) para realizar a comunicação entre as placas ESP32, deste modo, poderiam ser resolvidos problemas de conectividade em locais com má ou nenhuma conexão Wi-Fi. O nome do arquivo é esse por conta de se tratar do código utilizado nas ESP32 que ficam com sensores e que devem enviar os dados de umidade para uma ESP32 central, a qual recebe o código do arquivo [_serverRele.ino_](./esp32/serverRele/serverRele.ino).

* **googleSheets_watch_dog:** Código feito para controlar o sistema com uma EPS32, ligada a um sensor e ao relé. Foi o último código sob testes, os quais tiveram como objetivos: averiguar o funcionamento do sensor HD-38 e o funcionamento do Watch Dog. O Watch Dog é basicamente uma forma de prevenir travamentos na ESP32 a partir da programação, é possível também fazer isso através de hardware, mas inicialmente optou-se pela opção presente no código. O Watch Dog basicamente monitora se a ESP32 deixa de realizar as ações esperadas por determinado tempo, quando isso ocorre, a placa é reiniciada e volta a funcionar normalmente.

* **guardadorPlanilha:** Feito quando se decidiu utilizar o protocolo [ESP-NOW](https://www.espressif.com/en/solutions/low-power-solutions/esp-now) para realizar a comunicação entre as placas ESP32, deste modo, poderiam ser resolvidos problemas de conectividade em locais com má ou nenhuma conexão Wi-Fi. Esse código é colocado na ESP32 que recebe valores do sistema e fica em um local com boa conexão wifi, foi pensado principalemente para testes e armazenamento de dados na planilha do sistema de irrigação utilizada no projeto. A estratégia na qual está inserida a lógica desse código visa diminuir problemas de envio de informações para a planilha. O código não foi totalmente finalizado, assim como o [_serverRele.ino_](./esp32/clientSensor/clientSensor.ino). Foram pensadas mudanças para o código, até mesmo o abandono do ESP-NOW ou utilização do qual de outra maneira.

* **leituraDigitalSensorBrabo:** Código bastante simples que printa as leituras digitais do sensor HD-38.

* **onlyTestConnect:** Código bastante simples que somente executa a função de conexão wifi.

* **serverRele:** Feito quando se decidiu utilizar o protocolo [ESP-NOW](https://www.espressif.com/en/solutions/low-power-solutions/esp-now) para realizar a comunicação entre as placas ESP32, deste modo, poderiam ser resolvidos problemas de conectividade em locais com má ou nenhuma conexão Wi-Fi. O nome do arquivo é esse por conta de se tratar do código utilizado na ESP32 que controla o relé e, portanto, controla a ativação da irrigação conforme as informações recebidas por ESP32 ligadas a sensores, as quais recebem o código do arquivo [_serverRele.ino_](./esp32/clientSensor/clientSensor.ino). A estrutura que estava sendo criada para utilização desses códigos era: clientSensor envia dados para serverRele, serverRele envia dados para [_guardadorPlanilha_](./esp32/guardadorPlanilha/guardadorPlanilha.ino), guardadorPlanilha envia dados para a planilha. Contudo, os códigos não foram totalmente finalizados e houve cogitação de outras ideias, até mesmo o abandono do ESP-NOW ou utilização do qual de outra maneira. Essa estrutura foi pensada incialmente desse modo com o objeivo de alocar a ESP32 _guardadorPlanilha_ em um local onde a conexão com Wi-Fi fosse melhor, podendo receber os dados de irrigação e enviá-los com menos problemas.

* **testaNovos:** Feito para testar relés e sensores novos. A cada segundo o código realiza leituras com o sensor e "liga/desliga" o relé.

* **ThreadsEsp32:** Código simples para teste do funcionamento da ESP32 utilizando mais de uma thread.

## media_ate_o_limite

Contém um código para ser utilizado com dois sensores. O código foi foi escrito com o objetivo de realizar testagens com dois sensores conectados em uma mesma placa, executando uma média entre as leituras dos dois sensores e a tomando como o valor de umidade final.

## limit 

Basicamente, o primeiro código feito para funcionar somnte com um limite de umidade, descartando a ideia de uma variável com tempo de irrigação pré definido. Anteriormente ([rega_v1](./rega_v1/rega_v1.ino)), o protótipo - assim que percebia uma baixa umidade - realizava a irrigação por um tempo determinado em código. Com a adição do _limit_, o sistema passou a finalizar a irrigação somente quando houvesse leitura de um valor satisfatório de umidade.

## rega_v1

Primeiro código utilizado nos protótipos, seguia uma lógica com tempo de irrigação pré determinado, a qual posteriormente foi substituída com a leitura de valores de umidade até que se atingisse um satisfatório e, portanto, se identificasse a possibilidade de parar a irrigação.

* **rega_v2_sensor_capacitivo:** Código feito para os primeiros testes realizados com sensor capacitivo de umidade do solo.

* **rega_v3_media_de_umidade_capacitivo:** Código feito para testes com dois sensores capacitivos. Com as leituras dos sensores, é feita uma média tomada como valor final de umidade, a irrigação nesse código ainda é realizada através de um tempo pré determinado de irrigação.

## testaLeitura

Código para testar como as oscilações nas leituras de um sensor se comportam em diferentes tempos. O código mostra como foram as leituras feitas pelo sensor a partir do mesmo solo, num intervalo de tempo definido no código, além de calcular uma média das leituras. O código foi útil para determinar qual seria o tempo utilizado na função _getMoistureAvg_, pois ajudam a entender se as oscilações nas leituras, em determinado tempo, são ou não prejudiciais ao funcionamento do sistema.

## testaMUITObasicoArduino

Como o nome sugere, é bem básico, a cada segundo, faz leituras de umidade e as printa na tela. Código utilizado cotidianamente para testar sensores novos, averiguar funcionamento do hardware, entre outros.


