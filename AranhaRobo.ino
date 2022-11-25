// Inclusão das Bibliotecas necessárias

#include <SoftwareSerial.h>
#include <Servo.h>
#include <ArduinoJson.h>

// Declara a pinagem do bluetooth e dos servo motores

SoftwareSerial btserial(12, 13);

int pinleg1 = 10;
int pinleg2 = 11;
int pinleg3 = 6;
int pinleg4 = 9;

int pinfeet1 = 7;
int pinfeet2 = 8;
int pinfeet3 = 3;
int pinfeet4 = 5;

// Cria dados para armazenar as informações da Cristal em um JSON

StaticJsonDocument<256> jsondata;

// Declara as variáveis padrões para o programa

String btdata = "";
String jsoninfo = "";
String response = "";
bool is_iddle = false;

// Cria as variáveis para controlar os servo motores

Servo leg1;
Servo leg2;
Servo leg3;
Servo leg4;

Servo feet1;
Servo feet2;
Servo feet3;
Servo feet4;

void setup() { // Aplica as configurações e inicia o Arduino

  // Inicia os monitores seriais do Módulo Bluetooth e USB

  Serial.begin(9600);
  btserial.begin(9600);

  // Envia um sinal pelo Serial Bluetooth para realizar a ação no Aplicativo

  btserial.println("m");
  delay(100);
  btserial.println("q");

  // Cria as informações padrões das funções no documento JSON

  jsondata["walk_mode"] = 0;
  jsondata["tilt"] = false;
  jsondata["dance"] = false;
  jsondata["hello"] = false;
  jsondata["connect"] = false;
  ResetStates();

  serializeJson(jsondata, jsoninfo); // Converte as informações do JSON para ser exibido de maneira correta
  //Serial.println(jsoninfo);

  // Atribui cada servo motor ao seu respectivo pino do Arduino

  leg1.attach(pinleg1);
  leg2.attach(pinleg2);
  leg3.attach(pinleg3);
  leg4.attach(pinleg4);

  feet1.attach(pinfeet1);
  feet2.attach(pinfeet2);
  feet3.attach(pinfeet3);
  feet4.attach(pinfeet4);

  Initialize(); // Executa a Função para iniciar a Cristal na posição padrão
}

void loop() { // Roda o programa principal que faz o gerenciamento do comportamento da Cristal


  while (btserial.available() > 0) { // Verifica se há texto pendente disponível no Monitor serial Bluetooth
    btdata = (char)btserial.read(); // Salva o texto pendente em uma variável para utilizar no resto do código
    //while (btserial.available() > 0) btserial.read(); // Verifica se possui textos a mais no serial e limpa caso houver
  }

  if (btdata != NULL) { // Verifica se o comando não está em branco e pula a etapa de definição de funções caso estiver

    ResetStates(); // Limpa todos os as funções anteriores salvas para receber comandos novos

    is_iddle = false; // Retira o estado "aguardando por comando" da Cristal
    //response = btdata; // Define a resposta padrão para indicar a ação da Cristal

    Serial.println("Comando Recebido: \"" + btdata + "\""); // Exibe no Monitor Serial USB o comando recebido pelo Monitor Serial Bluetooth
    //Serial.println(jsoninfo); // Exibe no Monitor Serial USB o estado das variáveis JSON salvas

    // Faz a verificação dos comandos recebidos e o gerenciamento das funções habilitadas em suas variáveis

    if (btdata == "C") {
      jsondata["connect"] = true;
    } else if (btdata == "S") {
      jsondata["iddle"] = true;
    } else if (btdata == "F") {
      if (jsondata["tilt"] == false) jsondata["walk_front"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_front"] = true;
    } else if (btdata == "B") {
      if (jsondata["tilt"] == false) jsondata["walk_back"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_back"] = true;
    } else if (btdata == "L") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["turn_left"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_left"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_left"] = true;
    } else if (btdata == "R") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["turn_right"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_right"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_right"] = true;
    } else if (btdata == "G") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["walk_front_left"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_turn_front_left"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_front_left"] = true;
    } else if (btdata == "I") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["walk_front_right"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_turn_front_right"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_front_right"] = true;
    } else if (btdata == "H") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["walk_back_left"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_turn_back_left"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_back_left"] = true;
    } else if (btdata == "J") {
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 0) jsondata["walk_back_right"] = true;
      if (jsondata["tilt"] == false && jsondata["walk_mode"] == 1) jsondata["walk_turn_back_right"] = true;
      if (jsondata["tilt"] == true) jsondata["tilt_back_right"] = true;
    } else if (btdata == "w") {
      jsondata["walk_mode"] = 0;
      response = "q";
    } else if (btdata == "W") {
      jsondata["walk_mode"] = 1;
      response = "Q";
    } else if (btdata == "u") {
      response = "m";
      jsondata["tilt"] = false;
    } else if (btdata == "U") {
      jsondata["tilt"] = true;
      response = "M";
    } else if (btdata == "X") {
      jsondata["dance"] = true;
    } else if (btdata == "V") {
      jsondata["hello"] = true;
    }

    btdata = ""; // Limpa a variável para poder armazenar um novo valor assim que receber.
  } 
  
  if (is_iddle == false && response != "") {
    is_iddle = true;
    //Serial.println("Resposta: " + response); // Envia um sinal no Serial USB apenas para fins de teste e monitoramento
    btserial.println(response); // Envia um sinal pelo Serial Bluetooth para realizar a ação no Aplicativo
    response = "";
    //while(btserial.available()>0) btserial.read(); // Verifica se há informações no Serial Bluetooth e limpa os dados caso houver
  }
  // Verifica as variáveis salvas e executa a(s) funções habilitadas

  if (jsondata["connect"]) {
    ConnectResponse();
  } else if (jsondata["iddle"]) {
    Iddle();
  } else if (jsondata["turn_left"]) {
    TurnLeft();
  } else if (jsondata["turn_right"]) {
    TurnRight();
  } else if (jsondata["walk_front"]) {
    WalkFront();
  } else if (jsondata["walk_back"]) {
    WalkBack();
  } else if (jsondata["walk_left"]) {
    WalkLeft();
  } else if (jsondata["walk_right"]) {
    WalkRight();
  } else if (jsondata["tilt_front"]) {
    TiltFront();
  } else if (jsondata["tilt_back"]) {
    TiltBack();
  } else if (jsondata["tilt_left"]) {
    TiltLeft();
  } else if (jsondata["tilt_right"]) {
    TiltRight();
  } else if (jsondata["tilt_front_left"]) {
    TiltFrontLeft();
  } else if (jsondata["tilt_front_right"]) {
    TiltFrontRight();
  } else if (jsondata["tilt_back_left"]) {
    TiltBackLeft();
  } else if (jsondata["tilt_back_right"]) {
    TiltBackRight();
  } else if (jsondata["hello"]) {
    Hello();
  } else if (jsondata["dance"]) {
    Dance();
  }
}

// Função para mover todos os Eixos (Pernas e Joelhos) da Cristal ao mesmo tempo

void MoveAll(int leg1val, int leg2val, int leg3val, int leg4val, int feet1val, int feet2val, int feet3val, int feet4val) {
  if (leg1val != -1) leg1.write(leg1val);
  if (leg2val != -1) leg2.write(leg2val);
  if (leg3val != -1) leg3.write(leg3val);
  if (leg4val != -1) leg4.write(leg4val);
  if (feet1val != -1) feet1.write(feet1val);
  if (feet2val != -1) feet2.write(feet2val);
  if (feet3val != -1) feet3.write(feet3val);
  if (feet4val != -1) feet4.write(feet4val);
}

// Função para mover todos as Pernas da Cristal ao mesmo tempo

void MoveLegs(int leg1val, int leg2val, int leg3val, int leg4val) {
  if (leg1val != -1) leg1.write(leg1val);
  if (leg2val != -1) leg2.write(leg2val);
  if (leg3val != -1) leg3.write(leg3val);
  if (leg4val != -1) leg4.write(leg4val);
}

// Função para mover todos os Joelhos da Cristal ao mesmo tempo

void MoveFeets(int feet1val, int feet2val, int feet3val, int feet4val) {
  if (feet1val != -1) feet1.write(feet1val);
  if (feet2val != -1) feet2.write(feet2val);
  if (feet3val != -1) feet3.write(feet3val);
  if (feet4val != -1) feet4.write(feet4val);
}

/* Função executada ao inicializar o arduino, 
colocando a Cristal em sua posição inicial*/

void Initialize() {
  MoveLegs(90, 90, 90, 90);
  MoveFeets(70, 70, 70, 70);
}

// Função executada para enviar um sinal pelo Serial Bluetooth e resetar o estado dos ícones no Aplicativo

void ConnectResponse() {
  jsondata["walk_mode"] = 0;
  jsondata["tilt"] = false;
  jsondata["connect"] = false;
  btserial.println("m");
  delay(100);
  btserial.println("q");
}

// Função executada sempre que um novo comando é recebido para limpar as variáveis salvas anteriormente

void ResetStates() {
  jsondata["iddle"] = false;
  jsondata["turn_left"] = false;
  jsondata["turn_right"] = false;

  jsondata["walk_front"] = false;
  jsondata["walk_back"] = false;
  jsondata["walk_left"] = false;
  jsondata["walk_right"] = false;

  jsondata["tilt_front"] = false;
  jsondata["tilt_back"] = false;
  jsondata["tilt_left"] = false;
  jsondata["tilt_right"] = false;

  jsondata["tilt_front_left"] = false;
  jsondata["tilt_front_right"] = false;
  jsondata["tilt_back_left"] = false;
  jsondata["tilt_back_right"] = false;
}

// Sequência executada para a Cristal ficar parada

void Iddle() {
  MoveAll(90, 90, 90, 90, 70, 70, 70, 70);
  jsondata["iddle"] = false;
}

// Sequência executada para dar um passo para Frente

void WalkFront() {
  MoveLegs(45, 135, 135, 45);
  delay(50);
  feet3.write(30);
  delay(50);
  leg3.write(45);
  delay(50);
  feet3.write(70);
  delay(50);
  feet4.write(30);
  delay(50);
  leg4.write(135);
  delay(50);
  feet4.write(70);
  delay(50);
  feet2.write(30);
  delay(50);
  leg2.write(90);
  delay(50);
  feet2.write(70);
  delay(50);
  feet1.write(30);
  delay(50);
  leg1.write(90);
  delay(50);
  feet1.write(70);
  delay(50);
}

// Sequência executada para dar um passo para Trás

void WalkBack() {
  MoveLegs(135, 45, 45, 135);
  delay(50);
  feet1.write(30);
  delay(50);
  leg1.write(45);
  delay(50);
  feet1.write(70);
  delay(50);
  feet2.write(30);
  delay(50);
  leg2.write(135);
  delay(50);
  feet2.write(70);
  delay(50);
  feet4.write(30);
  delay(50);
  leg4.write(90);
  delay(50);
  feet4.write(70);
  delay(50);
  feet3.write(30);
  delay(50);
  leg3.write(90);
  delay(50);
  feet3.write(70);
  delay(50);
}

// Sequência executada para dar um passo para Esquerda

void WalkLeft() {
  MoveLegs(45, 45, 135, 135);
  delay(50);
  feet4.write(30);
  delay(50);
  leg4.write(45);
  delay(50);
  feet4.write(70);
  delay(50);
  feet1.write(30);
  delay(50);
  leg1.write(135);
  delay(50);
  feet1.write(70);
  delay(50);
  feet3.write(30);
  delay(50);
  leg3.write(90);
  delay(50);
  feet3.write(70);
  delay(50);
  feet2.write(30);
  delay(50);
  leg2.write(90);
  delay(50);
  feet2.write(70);
  delay(50);
}

// Sequência executada para dar um passo para Direita

void WalkRight() {
  MoveLegs(135, 135, 45, 45);
  delay(50);
  feet2.write(30);
  delay(50);
  leg2.write(45);
  delay(50);
  feet2.write(70);
  delay(50);
  feet3.write(30);
  delay(50);
  leg3.write(135);
  delay(50);
  feet3.write(70);
  delay(50);
  feet1.write(30);
  delay(50);
  leg1.write(90);
  delay(50);
  feet1.write(70);
  delay(50);
  feet4.write(30);
  delay(50);
  leg4.write(90);
  delay(50);
  feet4.write(70);
  delay(50);
}

// Sequência executada para Inclinar para Frente

void TiltFront() {
  MoveFeets(0, 0, 90, 90);
}

// Sequência executada para Inclinar para Trás

void TiltBack() {
  MoveFeets(90, 90, 0, 0);
}

// Sequência executada para Inclinar para Esquerda

void TiltLeft() {
  MoveFeets(90, 0, 0, 90);
}

// Sequência executada para Inclinar para Direita

void TiltRight() {
  MoveFeets(0, 90, 90, 0);
}

// Sequência executada para Inclinar para Diagonal Frente-Esquerda

void TiltFrontLeft() {
  MoveFeets(45, 0, 45, 90);
}

// Sequência executada para Inclinar para Diagonal Frente-Direita

void TiltFrontRight() {
  MoveFeets(0, 45, 90, 45);
}

// Sequência executada para Inclinar para Diagonal Trás-Esquerda

void TiltBackLeft() {
  MoveFeets(90, 45, 0, 45);
}

// Sequência executada para Inclinar para Diagonal Trás-Direita

void TiltBackRight() {
  MoveFeets(45, 90, 45, 0);
}

// Sequência executada para girar em seu próprio Eixo para a Esquerda

void TurnLeft() {
  feet1.write(0);
  delay(50);
  leg1.write(135);
  delay(50);
  feet1.write(70);
  delay(100);
  feet2.write(0);
  delay(50);
  leg2.write(135);
  delay(50);
  feet2.write(70);
  delay(100);
  feet3.write(0);
  delay(50);
  leg3.write(135);
  delay(50);
  feet3.write(70);
  delay(100);
  feet4.write(0);
  delay(50);
  leg4.write(135);
  delay(50);
  feet4.write(70);
  delay(100);
  MoveLegs(90, 90, 90, 90);
  delay(100);
}

// Sequência para girar em seu próprio Eixo para a Direita

void TurnRight() {
  feet2.write(0);
  delay(50);
  leg2.write(45);
  delay(50);
  feet2.write(70);
  delay(100);
  feet1.write(0);
  delay(50);
  leg1.write(45);
  delay(50);
  feet1.write(70);
  delay(100);
  feet4.write(0);
  delay(50);
  leg4.write(45);
  delay(50);
  feet4.write(70);
  delay(100);
  feet3.write(0);
  delay(50);
  leg3.write(45);
  delay(50);
  feet3.write(70);
  delay(100);
  MoveLegs(90, 90, 90, 90);
  delay(100);
}

// Sequência executada para realizar movimentos de dança

void Dance() {
  MoveFeets(90, 90, 90, 90);
  MoveLegs(0, 0, 0, 0);
  delay(250);
  MoveLegs(180, 180, 180, 180);
  delay(250);
  MoveLegs(0, 0, 0, 0);
  delay(250);
  MoveLegs(180, 180, 180, 180);
  delay(250);
  MoveLegs(135, 45, 135, 45);
  delay(250);
  MoveFeets(0, 0, -1, -1);
  delay(250);
  MoveFeets(90, 90, -1, -1);
  delay(250);
  MoveFeets(0, 0, -1, -1);
  delay(250);
  MoveFeets(90, 90, -1, -1);
  delay(250);
  MoveFeets(-1, -1, 90, 90);
  delay(250);
  MoveFeets(-1, -1, 0, 0);
  delay(250);
  MoveFeets(-1, -1, 90, 90);
  delay(250);
  MoveFeets(0, 0, 0, 0);
  delay(250);
  MoveFeets(90, 90, 90, 90);
  delay(250);
  MoveFeets(0, 0, 0, 0);
  delay(250);
  MoveFeets(90, 90, 90, 90);
  delay(250);
  MoveLegs(45, 135, 45, 135);
  delay(250);//
  MoveFeets(-1, 0, 0, -1);
  delay(250);
  MoveFeets(-1, 90, 90, -1);
  delay(250);
  MoveFeets(-1, 0, 0, -1);
  delay(250);
  MoveFeets(-1, 90, 90, -1);
  delay(250);
  MoveFeets(0, -1, -1, 0);
  delay(250);
  MoveFeets(90, -1, -1, 90);
  delay(250);
  MoveFeets(0, -1, -1, 0);
  delay(250);
  MoveFeets(90, -1, -1, 90);
  delay(250);//
  MoveFeets(0, 0, 0, 0);
  delay(250);
  MoveFeets(90, 90, 90, 90);
  delay(250);
  MoveFeets(0, 0, 0, 0);
  delay(250);
  MoveFeets(90, 90, 90, 90);
  delay(250);
  MoveFeets(0, 0, 0, 0);
  delay(250);
  MoveAll(90, 90, 90, 90, 70, 70, 70, 70);
  delay(250);
  jsondata["dance"] = false;
}

// Sequência executada para realizar o movimento de acenar

void Hello() {
  MoveFeets(90, 90, 0, 0);
  delay(100);
  feet1.write(0);
  delay(250);
  leg1.write(150);
  delay(100);
  leg1.write(120);
  delay(100);
  leg1.write(150);
  delay(100);
  leg1.write(120);
  delay(150);
  leg1.write(90);
  feet1.write(70);
  delay(100);
  MoveFeets(70, 70, 70, 70);
  jsondata["hello"] = false;
}