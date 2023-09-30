#pragma once

#include "EncryptedLoRaClient.h"
#include "LoRaClient.h"
#include "../crypto/SHA256.h"

LoRaWrapper::EncryptedLoRaClient::EncryptedLoRaClient()
{
  _scanning = false;
  _broadcasting = false;
  _peerCount = 0;
  _peers = new Peer *[MAX_PEERS];
  _rsaKey = new RSAKeyPair();
}

LoRaWrapper::EncryptedLoRaClient::EncryptedLoRaClient(String deviceID, SPIType spiType, Logger *logger) : EncryptedLoRaClient()
{
  _logger = logger;
  _logger->info("Instantiating LoRaClient...");
  _lora = new LoRaWrapper::LoRaClient(433E6, 0x12, spiType);
  _selfID = deviceID;
  _broadcastPackets = Packet::fromPayload(new Payload("broadcast_self", "", _selfID), _selfID, "*");
  _broadcastTask = new BackgroundTask(
      "BroadcastTask", [&]()
      {
       for (int i = 0; i < _broadcastPackets.size(); i++)
       {
        String pak = _broadcastPackets[i]->toJSON();
        _logger->info("[*]: " + pak);
         //sendPacket(pak);
       }
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

void LoRaWrapper::EncryptedLoRaClient::broadcastPacket(Packet *packet)
{
  _lora->send(packet->toJSON());
}