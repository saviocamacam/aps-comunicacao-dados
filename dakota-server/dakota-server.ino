#include <SPI.h>
#include "RF24.h"

/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
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
  //SERVIDOR EM MODO DE ESCUTA
  if (Serial.available())
  {
    if (currentMode == listening)
    {
      Serial.println(F("*** MUDANDO SERVIDOR PARA MODO TRANSMISSÃO"));
    }
    else if (role == transmitting)
    {
      Serial.println(F("*** MUDANDO SERVIDOR PARA MODO LEITURA"));
    }
  }
}
