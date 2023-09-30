#pragma once

#include "Payload.h"
#include "../../crypto/SHA256.h"
#include "Packet.h"
#include "Payload.h"
#include "../../crypto/SHA256.h"

namespace Data
{
  Payload::Payload() : action(""), method(""), params("")
  {
    
  }

  Payload::Payload(String action, String method, String params) : action(action), method(method), params(params)
  {
    
  }

  Payload::Payload(String action, String method) : Payload(action, method, "")
  {
    
  }

  Payload::Payload(String action) : Payload(action, "", "")
  {
    
  }

  String Payload::toJSON()
  {
    return "{\"action\":\"" + action + "\",\"method\":\"" + method + "\",\"params\":\"" + params + "\",\"checksum\":\"" + getChecksum() + "\"}";
  }

  String Payload::toUncheckedJSON() const
  {
	return "{\"action\":\"" + action + "\",\"method\":\"" + method + "\",\"params\":\"" + params + "\"}";
  }

  Payload *Payload::fromCheckedJSON(String json)
  {
      int checksumStart = json.indexOf("\"checksum\":\"") + 11;
      int checksumEnd = json.indexOf("\"", checksumStart);
      if (checksumStart == -1 || checksumEnd == -1) {
          return nullptr; 
      }
      String providedChecksum = json.substring(checksumStart, checksumEnd);
      String newAction = "";
      String newMethod = "";
      String newParams = "";
      int actionStart = json.indexOf("\"action\":\"") + 9;
      int actionEnd = json.indexOf("\"", actionStart);
      if (actionStart != -1 && actionEnd != -1) {
          newAction = json.substring(actionStart, actionEnd);
      }
      int methodStart = json.indexOf("\"method\":\"") + 9;
      int methodEnd = json.indexOf("\"", methodStart);
      if (methodStart != -1 && methodEnd != -1) {
          newMethod = json.substring(methodStart, methodEnd);
      }
      int paramsStart = json.indexOf("\"params\":\"") + 9;
      int paramsEnd = json.indexOf("\"", paramsStart);
      if (paramsStart != -1 && paramsEnd != -1) {
          newParams = json.substring(paramsStart, paramsEnd);
      }
      Payload *result = new Payload(newAction, newMethod, newParams);
      if (result->getChecksum() != providedChecksum) {
          return nullptr;
      }
      return result;
  }


  String Payload::getChecksum()
  {
      return SHA256::hmac_short(this->toUncheckedJSON(), PSK);
  }
};
