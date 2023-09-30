#pragma once

#include "LoRaWrapper.h"

class LoRaWrapper::LoRaClient
{
private:
  int _syncWord;
  long _frequency;
  static void (*_callback)(String message);
  static void _onReceive(int packetSize);
  LoRaClient(long frequency, int syncWord);

public:
  LoRaClient(long frequency, int syncWord, int sck, int miso, int mosi, int ss, int rst, int dio0);
  LoRaClient(long frequency, int syncWord, SPIType spiType);
  void send(String message);
  void send(String message, int syncWord, bool persistSyncWord);
  void onMessage(void (*onMessageReceived)(String message));
  void setSyncWord(int syncWord);
  void setSpreadingFactor(int sf);
  void setSignalBandwidth(long sbw);
  void setCodingRate4(int denominator);
  void setPreambleLength(long length);
  void setTxPower(int level);
};
