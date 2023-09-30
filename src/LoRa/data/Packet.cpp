#pragma once

#include "Packet.h"

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

  /**
   * @brief Calculates the length of the bloat of the packet. This is used for calculating the maximum payload size. ((maximum payload size / packet) = MAX_PACKET_SIZE - getBloatLength())
   */
  int Packet::getBloatLength(String source, String destination, int packetID, int index, int total)
  {
    return ("{\"source\":\"" + source + "\",\"destination\":\"" + destination + "\",\"payload\":{},\"packetID\":" + packetID + ",\"index\":" + index + ",\"total\":" + total + "}").length();
  }

  String Packet::toJSON()
  {
    return "{\"source\":\"" + source + "\",\"destination\":\"" + destination + "\",\"payload\":" + payload->toJSON() + "\",\"packetID\":" + _packetID + ",\"index\":" + index + ",\"total\":" + total + "}";
  }

  std::vector<Packet *> Packet::fromPayload(Payload *payload, String source, String destination)
  {
    int packetID = rand() ^ millis();
    int total = getBloatLength(source, destination, payloadSignature, packetID, 0, 0) / MAX_PACKET_SIZE + 1;
    std::vector<Packet *> packets;
    for (int i = 0; i < total; i++)
    {
      String payloadString = payload->toJSON().substring(i * MAX_PACKET_SIZE, (i + 1) * MAX_PACKET_SIZE);
      packets.push_back(new Packet(source, destination, new Payload("fragment", "reconstruct", payloadString), signingKey->sign(payloadString), packetID, i, total));
    }
    return packets;
  }
};