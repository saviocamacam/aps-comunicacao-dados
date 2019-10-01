#include <SPI.h>
#include "RF24.h"

/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
// ENDEREÇOS DE RÁDIO (PIPES) PARA DOIS NÓS SE COMUNICAREM
byte addresses[][11] = {"F0F0F0F0D2", "F0F0F0F0E1"};

//definindo tamanho da matriz e incializando-a com zero
#define LINHA 15
#define COLUNA 2
byte tabela[LINHA][COLUNA] = {{0}};

// DEFININDO OS MODOS QUE O NÓ PODE ASSUMIR
typedef enum
{
  bla = 200,
  transmitting = 1,
  listening
} mode;

mode currentMode = listening;
const char *myAddress;
 
 //funções manipulcao matriz de ITs
byte getIt(byte mac){
  for(int i = 1; i < LINHA; i ++){
      if(tabela[i] == mac){
        return i;
      }
  }
  return addIt(mac);
}

byte addIt(byte mac){
  for (int i = 0; i < LINHA; i++){
    if(tabela[i] == 0){
      tabela[i] = mac;
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
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("RF24/Dakota-server"));
  dtcp();

  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();
  // DESABILITA O MODO AUTO-ACK
  radio.setAutoAck(false);
  //HABILITA O MODO CARGA ÚTIL DINÂMICA
  radio.enableDynamicPayloads();
  //AMBOS OS RADIOS OUVEM OS MESMOS PIPES, MAS EM ENDEREÇOS OPOSTOS
  radio.openWritingPipe(addresses[1]);
  //ABRE UM PIPE DE LEITURA NO ENDEREÇO 0, PIPE 1
  radio.openReadingPipe(1, addresses[0]);
  radio.startListening();
}

void loop()
{

  // MODO DE TRANSMISSÃO
  if (currentMode == transmitting)
  {
  }
  // MODO DE ESCUTA
  if (currentMode == listening)
  {
    byte pipeNo;
    while (radio.available(&pipeNo))
    {
      uint8_t payloadSize = radio.getDynamicPayloadSize();
      payload = (byte *)realloc(payload, payloadSize);
      radio.read(&payload, payloadSize);
      byte net = payload[0];
      byte message = payload[1];
      if (net == 10)
      {
        switch (message)
        {
        case 0:
          //GET ADDRESS MESSAGE
          mac = payload[2];
          break;

        default:
          break;
        }
      }
      for (int i = 0; i < payloadSize; i++)
      {
        Serial.println(payload[i]);
      }
    }
  }
}
