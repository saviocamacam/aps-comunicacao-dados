#include <SPI.h>
#include "RF24.h"
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/power.h>

//DEFINES
#define EEPROM_ADDR EEPROM[0]
#define MY_ADDR EEPROM.read(0X00);
// #define MY_ADDR 0X00

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

typedef enum
{
  getAddress,

} message;

mode currentMode = listening;
byte protocol = 100;
byte getAddress = 101;
byte mac = 0;
byte tp = 0;
byte mask_destination_tp = B00001111;
byte mask_source_tp = B11110000;
byte *message;

//FUNÇÃO QUE VERIFICA SE JÁ TENHO ENDEREÇO. SE NÃO, EU PEÇO; (Dynamic Thing Configuration Protocol)
void dtcp()
{
  Serial.println(F("RF24/Dakota-client setup DTCP Address"));
  if (tp == 0)
  {
    Serial.println(F("RF24/Dakota-client has no address"));
    mac = MY_ADDR;
    radio.stopListening();
    Serial.println(F("RF24/sending get address message"));
    message = (byte *)realloc(message, 3);
    if (radio.write(&message, sizeof(message)))
    {
    }
  }
  else
  {
    Serial.print(F("RF24/Dakota-client has "));
    Serial.print(mac);
    Serial.println(F(" address"));
  }
}

void setup()
{
  Serial.println(F("RF24/Dakota-client"));
  // put your setup code here, to run once:
  Serial.begin(115200);
  //HABILITA O MODO CARGA ÚTIL DINÂMICA
  radio.enableDynamicPayloads();
  // DESABILITA O MODO AUTO-ACK
  radio.setAutoAck(false);
  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();
  //AMBOS OS RADIOS OUVEM OS MESMOS PIPES, MAS EM ENDEREÇOS OPOSTOS
  radio.openWritingPipe(addresses[0]);
  //ABRE UM PIPE DE LEITURA NO ENDEREÇO 1, PIPE 1
  radio.openReadingPipe(1, addresses[1]);
  dtcp();
  radio.startListening();
}

void loop()
{
  // MODO DE TRANSMISSÃO
  if (currentMode == transmitting)
  {
    byte *payload;
    radio.stopListening(); //PARA DE OUVIR PARA PODER FALAR

    unsigned long time = micros(); // Record the current microsecond count
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
          byte mac = payload[2];
          break;

        default:
          break;
        }
      }
      Serial.println(payload);
    }
  }
}
