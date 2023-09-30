#pragma once

#include "LoRaWrapper.h"
#include "../crypto/RSAKeyPair.h"
#include "../crypto/Crypto.h"
#include "Peer.h"
#include "../BackgroundTasks/BackgroundTask.h"
using namespace Crypto;
#include "data/Packet.h"
#include "data/Compression.h"
using namespace Data;
#include "../logger/Logger.h"

#define MAX_PEERS 1
#define MAX_MESSAGE_LENGTH 255 // 255 bytes/packet message max for LoRa

class LoRaWrapper::EncryptedLoRaClient
{
private:
#pragma region base functionality

  Logger *_logger;
  EncryptedLoRaClient();
  LoRaWrapper::LoRaClient *_lora;
  bool _scanning;
  bool _broadcasting;
  Peer **_peers;
  int _peerCount;
  String _selfID;

#pragma endregion

#pragma region cryptography

  RSAKeyPair *_rsaKey;

#pragma endregion

#pragma region tasks
  BackgroundTask *_scanTask;
  BackgroundTask *_broadcastTask;
#pragma endregion

#pragma region default packets
  std::vector<Packet *> _broadcastPackets;
#pragma endregion

  void sendPacket(Packet *packet, String peerID);
  void sendPacketWithAck(Packet *packet, String peerID, int retries, int retryDelay);
  void sendPacketWithAck(Packet *packet, String peerID, int retries, int retryDelay, void (*onAck)(Packet *ack));
  void broadcastPacket(Packet *packet);

public:
  EncryptedLoRaClient(String deviceID, SPIType spiType, Logger *logger);
  void startScanAsync(void (*onPotentialPeerFound)(Peer *potentialPeer));
  void stopScan();
  void startBroadcastingSelfAsync();
  void stopBroadcastingSelf();
  void send(String message, String peerID);
  void sendWithAck(String message, String peerID, int retries, int retryDelay);
  void connectToPeer(String peerID);
  void disconnectFromPeer(String peerID);
};