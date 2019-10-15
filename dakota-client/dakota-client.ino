#include <SPI.h>
#include "RF24.h"
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/power.h>

//DEFINES
#define MY_ADDR EEPROM.read(0X0A)
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
} messages;

mode currentMode = listening;
byte protocol = 100;
byte mac = 0;
byte tp = 0;
byte mask_destination_tp = B00001111;
byte mask_source_tp = B11110000;
byte *message;

//FUNÇÃO QUE VERIFICA SE JÁ TENHO ENDEREÇO. SE NÃO, EU PEÇO; (Dynamic Thing Configuration Protocol)
void dtcp()
{
  Serial.println(F("RF24/Dakota-client setup DTCP Address"));
  while (tp == 0)
  {
    Serial.println(F("RF24/Dakota-client has no address"));
    mac = MY_ADDR;
    Serial.print(F("RF24/Dakota-client has "));
    Serial.print(mac);
    Serial.println(F(" mac"));
    radio.stopListening();
    Serial.println(F("RF24/sending get address message"));
    message = (byte *)realloc(message, 3);
    message[0] = 10;
    message[1] = 0;
    message[2] = mac;
    for (int i = 0; i < 3; i++)
      Serial.print(message[i], HEX);
    Serial.println();
    unsigned long time = micros();
    if (radio.write(&message, sizeof(message)))
    {
      radio.startListening();
      if (!radio.available())
      { // If nothing in the buffer, we got an ack but it is blank
        Serial.print(F("Got blank response. round-trip delay: "));
        Serial.print(micros() - time);
        Serial.println(F(" microseconds"));
      }
    }
    else
    {
      Serial.println(F("ATL - Falha ao enviar"));
    }
  }
  Serial.print(F("RF24/Dakota-client has "));
  Serial.print(mac);
  Serial.println(F(" address"));
}

void setup()
{
  Serial.println(F("RF24/Dakota-client"));
  Serial.println(MY_ADDR);
  Serial.begin(115200);
  // CONFIGURA PLACA E INICIA O RÁDIO
  radio.begin();

  dtcp();
  //HABILITA O MODO CARGA ÚTIL DINÂMICA
  radio.enableDynamicPayloads();
  // DESABILITA O MODO AUTO-ACK
  radio.setAutoAck(false);

  //AMBOS OS RADIOS OUVEM OS MESMOS PIPES, MAS EM ENDEREÇOS OPOSTOS
  radio.openWritingPipe(addresses[0]);
  //ABRE UM PIPE DE LEITURA NO ENDEREÇO 1, PIPE 1
  radio.openReadingPipe(1, addresses[1]);

  radio.startListening();
  dtcp();
}

void loop()
{
  byte *payload;
  // MODO DE TRANSMISSÃO
  if (currentMode == transmitting)
  {
    radio.stopListening(); //PARA DE OUVIR PARA PODER FALAR

    unsigned long time = micros(); // Record the current microsecond count
    if (radio.write(&message, sizeof(message)))
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
          radio.read(&message, 1);
          Serial.print(F("Resposta: "));
          // Serial.print(message);
          currentMode = listening;
          // radio.startListening();
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
