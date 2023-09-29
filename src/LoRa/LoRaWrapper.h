#ifndef LORAWRAPPER_H
#define LORAWRAPPER_H

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

class LoRaWrapper
{
public:
  enum SPIType
  {
    DefaultSPI,
    CustomSPI
  };
  class LoRaClient;
  class EncryptedLoRaClient;
};

#endif