#pragma once

#include <Arduino.h>
#include "Payload.h"
#include "../../crypto/RSAKeyPair.h"
#include "../../crypto/Crypto.h"
using namespace Crypto;
#include <vector>

#define MAX_PACKET_SIZE 200

namespace Data
{
  /**
   * @brief A packet of data to be sent over LoRa. The equivalent of a TCP/IP datagram.
   */
  class Packet
  {
  private:
    int _packetID;
    Packet();
    Packet(String source, String destination, Payload *payload, int packetID, int index, int total);
    static int getBloatLength(String source, String destination, int packetID, int index, int total);

  public:
    String source;
    String destination;
    Payload *payload;
    String payloadSignature;
    int index;
    int total;
    String toJSON();
    static std::vector<Packet *> fromPayload(Payload *payload, String source, String destination);
  };
};