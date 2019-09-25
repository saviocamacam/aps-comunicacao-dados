#include <SPI.h>
#include "RF24.h"

/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
// ENDEREÇOS DE RÁDIO (PIPES) PARA DOIS NÓS SE COMUNICAREM
byte addresses[][11] = {"F0F0F0F0D2", "F0F0F0F0E1"};
// DEFININDO OS MODOS QUE O NÓ PODE ASSUMIR
typedef enum
{
  transmitting = 1,
  listening
} mode;

mode currentMode = listening;
const char *myAddress;

//FUNÇÃO QUE INICIA E MANTÉM A TABELA DE ENDEREÇOS DOS CLIENTES;
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

  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();
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
  }
}
