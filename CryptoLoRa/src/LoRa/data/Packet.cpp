#pragma once

#include "Packet.h"
#include "Compressor.h"

namespace Data
{
  Packet::Packet()
  {
    source = "";
    destination = "";
    payload = nullptr;
    _packetID = 0;
    index = 0;
    total = 0;
  }

  Packet::Packet(String source, String destination, Payload *payload, int packetID, int index, int total)
  {
    this->source = source;
    this->destination = destination;
    this->payload = payload;
    this->_packetID = packetID;
    this->index = index;
    this->total = total;
  }

  String Packet::toJSON()
  {
    return "{\"source\":\"" + source + "\",\"destination\":\"" + destination + "\",\"payload\":" + payload->toJSON() + "\",\"packetID\":" + _packetID + ",\"index\":" + index + ",\"total\":" + total + "}";
  }

  std::vector<Packet *> Packet::fromPayload(Payload *payload, String source, String destination, RSAKeyPair* signingKey)
  {
    int packetID = rand() ^ millis();
    String json = "\"" + Base64::encode(payload->toJSON()) + "\"";
    int total = json.length() / MAX_PACKET_SIZE + 1;
    std::vector<Packet *> packets;
    for (int i = 0; i < total; i++)
    {
      String payloadString = json.substring(i * MAX_PACKET_SIZE, (i + 1) * MAX_PACKET_SIZE);
      packets.push_back(new Packet(source, destination, new Payload("fragment", "reconstruct", payloadString, signingKey), packetID, i, total));
    }
    return packets;
  }
};