#pragma once

#include "Packet.h"
#include "Compressor.h"
#include "../../crypto/SHA256.h"

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

  String Packet::toUncheckedJSON()
  {
    return "{\"source\":\"" + source + "\",\"destination\":\"" + destination + "\",\"payload\":" + payload->toJSON() + "\",\"packetID\":" + _packetID + ",\"index\":" + index + ",\"total\":" + total + "}";
  }

  String Packet::toJSON()
  {
      return "{\"source\":\"" + source + "\",\"destination\":\"" + destination + "\",\"payload\":" + payload->toJSON() + "\",\"packetID\":" + _packetID + ",\"index\":" + index + ",\"total\":" + total + ",\"checksum\":\"" + getChecksum() + "\"" + "}";
  }

  std::vector<Packet *> Packet::fromPayload(Payload *payload, String source, String destination)
  {
    int packetID = rand() ^ millis();
    String json = Base64::encode(payload->toJSON());
    int total = json.length() / MAX_PACKET_SIZE + 1;
    std::vector<Packet *> packets;
    for (int i = 0; i < total; i++)
    {
      String payloadString = json.substring(i * MAX_PACKET_SIZE, (i + 1) * MAX_PACKET_SIZE);
      packets.push_back(
          new Packet(
              source,
              destination, 
              new Payload("fragment", "reconstruct", payloadString), 
              packetID, 
              i, 
              total));
    }
    return packets;
  }

  String Packet::_extractField(String json, String key)
  {
      int start = json.indexOf("\"" + key + "\":\"") + key.length() + 4;
      int end = json.indexOf("\"", start);
      if (start == -1 || end == -1) {
          return "";
      }
      return json.substring(start, end);
  }

  Packet* Packet::fromCheckedJSON(String json)
  {
      // Extract the provided checksum
      String providedChecksum = _extractField(json, "checksum");
      if (providedChecksum == "") {
          return nullptr; // checksum field not found
      }

      // Initialize variables to hold the extracted values
      String newSource = _extractField(json, "source");
      String newDestination = _extractField(json, "destination");
      int newIndex = _extractField(json, "index").toInt();
      int newTotal = _extractField(json, "total").toInt();
      String newPayloadSignature = _extractField(json, "payloadSignature");
      String newPacketID = _extractField(json, "packetID");

      // More complex logic might be required to extract the Payload
      String payloadString = _extractField(json, "payload");
      Payload* newPayload = Payload::fromCheckedJSON(payloadString);

      // Create a new Packet object
      Packet* newPacket = new Packet(newSource, newDestination, newPayload, atoi(newPacketID.c_str()), newIndex, newTotal);

      // Compute the new checksum
      String newChecksum = SHA256::hmac_short(newPacket->toUncheckedJSON(), PSK);

      // Compare the computed checksum to the provided one
      if (newChecksum == providedChecksum) {
          return newPacket;
      }
      else {
          delete newPayload;  // Clean up allocated memory for payload
          delete newPacket;  // Clean up allocated memory for packet
          return nullptr;
      }
  }

  String Packet::getChecksum()
  {
      return SHA256::hmac_short(toUncheckedJSON(), PSK);
  }
};