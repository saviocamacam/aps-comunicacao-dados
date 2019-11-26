#include <SPI.h>
#include "RF24.h"
/*
  letras de controle:
  L = REDE
  S = Devolver endereços da rede

*/
/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
// ENDEREÇOS DE RÁDIO (PIPES) PARA DOIS NÓS SE COMUNICAREM
byte addresses[][6] = {"1Node", "2Node"};

// byte addresses[][11] = {"F0F0F0F0D2", "F0F0F0F0E1"};

//definindo tamanho da matriz e incializando-a com zero
#define LINHA 15
#define COLUNA 2
byte tabela[LINHA][COLUNA] = {{0x0}};
byte counter = 1;

byte enderecos[4] = {0x18, 0x1A, 0x2E, 0X05};
// enderecos[0] = 0x18;
// enderecos[1] = 0x1A; //servidor
// enderecos[2] = 0x2E;
// enderecos[3] = 0x05;





// DEFININDO OS MODOS QUE O NÓ PODE ASSUMIR
typedef enum
{
  transmitting = 1,
  listening
} mode;

mode currentMode = listening;
const char *myAddress;
byte mac;

//funções manipulcao matriz de ITs
byte getIt(byte mac)
{
  for (int i = 1; i < LINHA; i++)
  {
    if (tabela[i][0] == mac)
    {
      return i;
    }
  }
  return addIt(mac);
}

byte addIt(byte mac)
{
  for (int i = 0; i < LINHA; i++)
  {
    if (tabela[i][0] == 0)
    {
      tabela[i][0] = mac;
      return i;
    }
  }
}

//FUNÇÃO QUE INICIA E MANTÉM A TABELA DE ENDEREÇOS DOS CLIENTES (Dynamic Thing Configuration Protocol)
void dtcp()
{
  Serial.println(F("RF24/Dakota-server"));
}

const char *getMacAddress()
{
  // RETORNA ENDEREÇO FÍSICO QUE O PHS DEU PARA CADA PLACA
}

void setup()
{
  unsigned long previousMillis = millis();

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("RF24/Dakota-server"));
  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();
  radio.setChannel(60);

  // DESABILITA O MODO AUTO-ACK
  radio.setAutoAck(false);

  radio.enableAckPayload();
  //HABILITA O MODO CARGA ÚTIL DINÂMICA
  radio.enableDynamicPayloads();
  //AMBOS OS RADIOS OUVEM OS MESMOS PIPES, MAS EM ENDEREÇOS OPOSTOS
  radio.openWritingPipe(addresses[1]);
  //ABRE UM PIPE DE LEITURA NO ENDEREÇO 0, PIPE 1
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
  radio.writeAckPayload(1, &counter, 1);
  dtcp();
}

void loop()
{
  byte payload[30];
  // MODO DE TRANSMISSÃO
  if (currentMode == transmitting)
  {
  }
  // MODO DE ESCUTA
  if (currentMode == listening)
  {
    //Serial.println("dakotaRF24 -- listening mode");
    // Serial.println(radio.available());
    byte pipeNo, gotByte;
    // Serial.println("pipeNo\n");

    // Serial.println(pipeNo);
    while (radio.available(&pipeNo))
    {
      Serial.println("dakotaRF24 - listening mode WHILE - ");

      uint8_t payloadSize = radio.getDynamicPayloadSize();
      // payload = (byte *)realloc(payload, payloadSize);
      radio.read(payload, payloadSize);
      gotByte += 1;
      radio.writeAckPayload(pipeNo, &gotByte, 1);
      char net = payload[0];
      byte message = payload[1];
      if (net == 'L')
      {
        switch (message)
        {
        case 'S':
          //GET ADDRESS MESSAGE
          currentMode = transmitting;
          char str[8] = {'L', 'S', enderecos};
          if (radio.write(str, 8)){
             Serial.println(F("Got response bla 2"));
             radio.startListening();
             delay(5000);

    }
          // enderecos
          mac = payload[2];
          // byte ip = getIt(mac);

          break;

        default:
          break;
        }
      }
      Serial.print("size = ");

      unsigned long currentMillis = millis();

      Serial.println(payloadSize);
      for (int i = 0; i < payloadSize; i++)
      {
        if (i == 2)
        {
          Serial.print("paylod[");
          Serial.print(i);
          Serial.print("] = ");
          Serial.println(payload[i], HEX);
        }
        else
        {
          Serial.print("paylod[");
          Serial.print(i);
          Serial.print("] = ");
          Serial.println((char)payload[i]);
        }
      }
    }
  }
}
