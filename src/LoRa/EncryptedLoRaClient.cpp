#pragma once

#include "EncryptedLoRaClient.h"
#include "LoRaClient.h"

LoRaWrapper::EncryptedLoRaClient::EncryptedLoRaClient(String deviceID, SPIType spiType)
{
  _lora = new LoRaWrapper::LoRaClient(433E6, 0x12, spiType);
  _scanning = false;
  _broadcasting = false;
  _selfID = deviceID;
  _peerCount = 0;
  _peers = new Peer *[MAX_PEERS];
  _broadcastTask = new BackgroundTask(
      "BroadcastTask", [&]()
      {
        Serial.println("[Broadcast]: " + _selfID);
    _lora->send(_selfID);
    vTaskDelay(pdMS_TO_TICKS(1000)); });
  _scanTask = new BackgroundTask("ScanTask", []() {});
}

void LoRaWrapper::EncryptedLoRaClient::startBroadcastingSelfAsync()
{
  if (!_broadcasting)
  {
    _broadcasting = true;
    if (_broadcastTask != nullptr)
    {
      _broadcastTask->start();
    }
  }
}

void LoRaWrapper::EncryptedLoRaClient::stopBroadcastingSelf()
{
  if (_broadcasting)
  {
    _broadcasting = false;
    if (_broadcastTask != nullptr)
    {
      _broadcastTask->stop();
    }
  }
}

void LoRaWrapper::EncryptedLoRaClient::startScanAsync(void (*onPotentialPeerFound)(Peer *potentialPeer))
{
  if (!_scanning)
  {
    _scanning = true;
    if (_scanTask != nullptr)
    {
      _scanTask->start();
    }
  }
}

void LoRaWrapper::EncryptedLoRaClient::stopScan()
{
  if (_scanning)
  {
    _scanning = false;
    if (_scanTask != nullptr)
    {
      _scanTask->stop();
    }
  }
}
