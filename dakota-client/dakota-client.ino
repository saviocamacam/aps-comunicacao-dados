#include <SPI.h>
#include "RF24.h"
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/power.h>

//DEFINES
#define MY_ADDR 0X00
#define EEPROM_ADDR EEPROM[0]

/* CONFIGURAÇÃO DE HARDWARE: CONFIGURA O RÁDIO nRF24L01 EM BARRAMENTO SPI COM PINOS 7 & 8 */
RF24 radio(7, 8);
// ENDEREÇOS DE RÁDIO (PIPES) PARA DOIS NÓS SE COMUNICAREM
byte addresses[][11] = {"F0F0F0F0E1", "F0F0F0F0D2"};
// DEFININDO OS MODOS QUE O NÓ PODE ASSUMIR
typedef enum
{
  transmitting = 1,
  listening = 0,
} mode;

mode currentMode = listening;
byte myAddress = 0;
byte *data = 0B0;

//FUNÇÃO QUE VERIFICA SE JÁ TENHO ENDEREÇO. SE NÃO, EU PEÇO;
void dtcp()
{
  Serial.println(F("RF24/Dakota-client setup DTCP Address"));
  if (MY_ADDR != 0XFF)
  {
    myAddress = MY_ADDR;
  }
  else
  {
    //read from eeprom
    myAddress = EEPROM.read(EEPROM_ADDR);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("RF24/Dakota-client"));
  dtcp();

  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();
  //AMBOS OS RADIOS OUVEM OS MESMOS PIPES, MAS EM ENDEREÇOS OPOSTOS
  radio.openWritingPipe(addresses[0]);
  //ABRE UM PIPE DE LEITURA NO ENDEREÇO 1, PIPE 1
  radio.openReadingPipe(1, addresses[1]);
  radio.startListening();
}

void loop()
{
  // MODO DE TRANSMISSÃO
  if (currentMode == transmitting)
  {
    byte gotByte;
    radio.stopListening(); //PARA DE OUVIR PARA PODER FALAR

    if (radio.write(&data, sizeof(data)))
    {
      //VERIFICA SE BUFFER ESTA VAZIO
      if (!radio.available())
      {
        Serial.print(F("Resposta em branco. Espera: "));
        Serial.print(micros() - time);
        Serial.println(F(" micro-segundos"));
      }
      else
      {
        while (radio.available())
        {
          radio.read(&gotByte, 1);
          Serial.print(F("Resposta: "));
          Serial.print(gotByte);
          currentMode = listening;
          radio.startListening();
          Serial.print(F("> modo leitura"));
        }
      }
    }
    else
    {
      Serial.println(F("ATL - Falha ao enviar"));
    }
  }
  // MODO DE ESCUTA
  if (currentMode == listening)
  {
    byte pipeNo, gotByte;
    while (radio.available(&pipeNo))
    {
      radio.read(&gotByte, 1);
      radio.writeAckPayload(pipeNo, &gotByte, 1); //POR QUE O PROFESSOR DISSE PRA DESABILITAR O ACK
      Serial.println(gotByte);
    }
  }
}
