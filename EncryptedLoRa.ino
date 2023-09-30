#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include "src/LoRa/EncryptedLoRaClient.h"
#include "src/TFT/TFTDisplay.h"
#include "src/TFT/TFTDisplayExtensions.h"
#include "src/crypto/RSA2048.h"
#include "src/crypto/Crypto.h"
#include "src/logger/Logger.h"
using namespace Crypto;

const String _deviceID = "crooked_owl";
TFTDisplay *_tft = new TFTDisplay();
Logger *_logger = new Logger(_tft);
LoRaWrapper::EncryptedLoRaClient *_lora;

void setup()
{
  Serial.begin(115200);
  _logger->info("Starting...");
  // runTests();
  _logger->info("Initializing LoRa...");
  _lora = new LoRaWrapper::EncryptedLoRaClient(_deviceID, LoRaWrapper::SPIType::CustomSPI, _logger);
  _logger->info("LoRa ok");
  _lora->startBroadcastingSelfAsync();
  _logger->info("Broadcasting...");
}

void loop()
{
}

void runTests()
{
  runCryptoTests();
}

void runCryptoTests()
{
  _logger->info("Running crypto tests...");
  RSA2048::test(_logger);
}