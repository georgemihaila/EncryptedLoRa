#pragma once

#include "LoRaClient.h"

#define DEF_SS 5
#define DEF_RST 14
#define DEF_DIO0 2

#define CUSTOM_SCK 26
#define CUSTOM_MISO 21
#define CUSTOM_MOSI 27
#define CUSTOM_SS 22
#define CUSTOM_RST 12
#define CUSTOM_DIO0 25

LoRaWrapper::LoRaClient::LoRaClient(long frequency, int syncWord)
{
  _syncWord = syncWord;
  _frequency = frequency;
}

LoRaWrapper::LoRaClient::LoRaClient(long frequency, int syncWord, int sck, int miso, int mosi, int ss, int rst, int dio0) : LoRaWrapper::LoRaClient::LoRaClient(frequency, syncWord)
{
}

LoRaWrapper::LoRaClient::LoRaClient(long frequency, int syncWord, SPIType spiType) : LoRaWrapper::LoRaClient::LoRaClient(frequency, syncWord)
{
  switch (spiType)
  {
  case DefaultSPI:
    LoRa.setPins(DEF_SS, DEF_RST, DEF_DIO0);
    break;
  case CustomSPI:
    SPI.begin(CUSTOM_SCK, CUSTOM_MISO, CUSTOM_MOSI, CUSTOM_SS);
    LoRa.setPins(CUSTOM_SS, CUSTOM_RST, CUSTOM_DIO0);
    break;
  default:
    ESP.restart();
    break;
  }
  Serial.println("LoRa initializing...");
  while (!LoRa.begin(frequency))
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("LoRa up");
}

void LoRaWrapper::LoRaClient::send(String message)
{
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}

void LoRaWrapper::LoRaClient::send(String message, int syncWord, bool persistSyncWord = false)
{
  int currentSyncWord = syncWord;
  LoRa.setSyncWord(syncWord);
  send(message);
  if (!persistSyncWord)
  {
    LoRa.setSyncWord(currentSyncWord);
  }
}

void LoRaWrapper::LoRaClient::setSyncWord(int syncWord)
{
  _syncWord = syncWord;
  LoRa.setSyncWord(syncWord);
}

void LoRaWrapper::LoRaClient::setSpreadingFactor(int sf)
{
  LoRa.setSpreadingFactor(sf);
}

void LoRaWrapper::LoRaClient::setSignalBandwidth(long sbw)
{
  LoRa.setSignalBandwidth(sbw);
}

void LoRaWrapper::LoRaClient::setCodingRate4(int denominator)
{
  LoRa.setCodingRate4(denominator);
}

void LoRaWrapper::LoRaClient::setPreambleLength(long length)
{
  LoRa.setPreambleLength(length);
}

void LoRaWrapper::LoRaClient::setTxPower(int level)
{
  LoRa.setTxPower(level);
}

void LoRaWrapper::LoRaClient::onMessage(void (*onMessageReceived)(String message))
{
  _callback = onMessageReceived;
  LoRa.onReceive(_onReceive);
  LoRa.receive();
}

void LoRaWrapper::LoRaClient::_onReceive(int packetSize)
{
  char message[packetSize + 1];
  for (int i = 0; i < packetSize; i++)
  {
    message[i] = (char)LoRa.read();
  }
  message[packetSize] = '\0';
  if (_callback != nullptr)
    _callback(message);
}
