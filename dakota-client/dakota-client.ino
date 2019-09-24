#include <SPI.h>
#include "RF24.h"

/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
// DEFININDO OS MODOS QUE O NÓ PODE ASSUMIR
typedef enum
{
  transmitting = 1,
  listening = 0,
} mode;

mode currentMode = listening;
const char *myAddress;

//FUNÇÃO QUE VERIFICA SE JÁ TENHO ENDEREÇO. SE NÃO, EU PEÇO;
void dtcp()
{
  Serial.println(F("RF24/Dakota-client"));
}

const char *getMacAddress()
{
  // RETORNA ENDEREÇO FÍSICO QUE O PHS DEU PARA CADA PLACA
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("RF24/Dakota-client"));

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
  //CLIENTE EM MODO DE ESCUTA
  if (Serial.available())
  {
    if (currentMode == listening)
    {
      Serial.println(F("*** MUDANDO CLIENTE PARA MODO TRANSMISSÃO"));
    }
    else if (currentMode == transmitting)
    {
      Serial.println(F("*** MUDANDO CLIENTE PARA MODO LEITURA"));
    }
  }
}
